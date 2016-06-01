#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

//某类商品
class CommodityClass
{
public:
	CommodityClass();
	~CommodityClass();
	double m_Count = 1.0;//折扣(1.0为无折扣)
	double m_Gift = 0.5;//赠品买二赠一
	string m_Name;//商品名称
	string m_Unit;//商品数单位量
	double m_PerPrice;//商品单价
	string m_Type;//类别
};
CommodityClass::CommodityClass()
{
}
CommodityClass::~CommodityClass()
{
}

map<string, CommodityClass> CommTable;//商品总目录
map<string, int> InputData;//购买的商品清单
//拆分字符串
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
//测试输入的JSON数据
void GetInput(vector<string>& input)
{
	input.push_back("ITEM000001");
	input.push_back("ITEM000001");
	input.push_back("ITEM000001");
	input.push_back("ITEM000002");
	input.push_back("ITEM000003-2");
	input.push_back("ITEM000004");
	return;
}
//整理接收的JSON数据
void SortInput(const vector<string>& input)
{
	for(auto str:input)
	{
		int num = 1;
		std::string c = "-";
		std::vector<string> strV;
		SplitString(str, strV, c);
		if (strV.size() > 1)
		{
			num = atoi(strV[1].c_str());
		}
		if (!strV.empty())
		{
			map<string,int >::iterator iter_find;;
			iter_find = InputData.find(strV[0]);
			if (iter_find != InputData.end())
			{
				//若商品已存在，重置数量
				num = num + iter_find->second;
				InputData[strV[0]] = num;
			}
			else
			{
				InputData.insert(map<string, int>::value_type(strV[0], num));
			}
		}
	}
}
//控制台输出结算清单的文本
void PrintOut(map<string, int>& input)
{
	//std::cout << std::setprecision(4) << std::right << std::setw(10) << "***<没钱赚商店>购物清单***" << std::endl;;
	map<string, int> GiftComm;//买二赠一的商品
	double totalPrice = 0.0;
	double saveMoney = 0.0;
	cout << "***<没钱赚商店>购物清单***" << endl;
	for (auto obj : input)
	{
		CommodityClass& oneComm = CommTable[obj.first];
		int num = obj.second;
		int num_Gift = int(num*oneComm.m_Gift);//买二赠一
		cout << "名称：" << oneComm.m_Name << "，" << "数量：" << num + num_Gift << oneComm.m_Unit << "，";
		cout << "单价：" << fixed << setprecision(2) << oneComm.m_PerPrice << "(元)" << "，";
		if (num_Gift != 0)//享受买二赠一
		{
			GiftComm.insert(map<string, int>::value_type(obj.first, num_Gift));
			cout << "小计：" << oneComm.m_PerPrice*obj.second << "(元)";
			totalPrice += oneComm.m_PerPrice*obj.second;
			saveMoney += oneComm.m_PerPrice*num_Gift;
		}
		else
		{
			cout << "小计：" << oneComm.m_PerPrice*num*oneComm.m_Count << "(元)";
			totalPrice += oneComm.m_PerPrice*num*(1 - oneComm.m_Count);
			//享受折扣
			if (oneComm.m_Count < 1.0)
			{
				cout << "，" << "节省" << oneComm.m_PerPrice*num*oneComm.m_Count << "(元)";
				saveMoney += oneComm.m_PerPrice*num*oneComm.m_Count;
			}
		}
		cout << endl;
	}
	cout << "----------------------" << endl;
	if (!GiftComm.empty())
	{
		cout << "买二赠一商品：" << endl;
		for (auto obj : GiftComm)
		{
			CommodityClass& oneComm = CommTable[obj.first];
			int num = obj.second;
			cout << "名称：" << oneComm.m_Name << "，" << "数量：" << num << oneComm.m_Unit << endl;
		}
		cout << "----------------------" << endl;

	}
	cout << "总计：" << totalPrice << "元" << endl;
	if (saveMoney > 0.0)
	{
		cout << "节省：" << saveMoney << "元" << endl;
	}
	cout << "**********************" << endl;
}
int main()
{
	//没钱赚商店的所有商品数据
	CommodityClass one;
	{
		one.m_Gift = 0.5;
		one.m_Count = 1.0;
		one.m_Name = "可口可乐";
		one.m_PerPrice = 2.0;
		one.m_Type = "饮料";
		one.m_Unit = "瓶";
		CommTable["ITEM000001"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 0.9;
		one.m_Name = "苹果";
		one.m_PerPrice = 5.5;
		one.m_Type = "水果";
		one.m_Unit = "斤";
		CommTable["ITEM000002"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 1.0;
		one.m_Name = "羽毛球";
		one.m_PerPrice = 1.0;
		one.m_Type = "体育";
		one.m_Unit = "个";
		CommTable["ITEM000003"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 1.0;
		one.m_Name = "百事可乐";
		one.m_PerPrice = 2.0;
		one.m_Type = "饮料";
		one.m_Unit = "瓶";
		CommTable["ITEM000004"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 1.0;
		one.m_Name = "加多宝";
		one.m_PerPrice = 3.0;
		one.m_Type = "饮料";
		one.m_Unit = "瓶";
		CommTable["ITEM000005"] = one;
	}
	vector<string> Input;
	GetInput(Input);
	SortInput(Input);
	PrintOut(InputData);
	return true;
}