#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

//ĳ����Ʒ
class CommodityClass
{
public:
	CommodityClass();
	~CommodityClass();
	double m_Count = 1.0;//�ۿ�(1.0Ϊ���ۿ�)
	double m_Gift = 0.5;//��Ʒ�����һ
	string m_Name;//��Ʒ����
	string m_Unit;//��Ʒ����λ��
	double m_PerPrice;//��Ʒ����
	string m_Type;//���
};
CommodityClass::CommodityClass()
{
}
CommodityClass::~CommodityClass()
{
}

map<string, CommodityClass> CommTable;//��Ʒ��Ŀ¼
map<string, int> InputData;//�������Ʒ�嵥
//����ַ���
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
//���������JSON����
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
//������յ�JSON����
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
				//����Ʒ�Ѵ��ڣ���������
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
//����̨��������嵥���ı�
void PrintOut(map<string, int>& input)
{
	//std::cout << std::setprecision(4) << std::right << std::setw(10) << "***<ûǮ׬�̵�>�����嵥***" << std::endl;;
	map<string, int> GiftComm;//�����һ����Ʒ
	double totalPrice = 0.0;
	double saveMoney = 0.0;
	cout << "***<ûǮ׬�̵�>�����嵥***" << endl;
	for (auto obj : input)
	{
		CommodityClass& oneComm = CommTable[obj.first];
		int num = obj.second;
		int num_Gift = int(num*oneComm.m_Gift);//�����һ
		cout << "���ƣ�" << oneComm.m_Name << "��" << "������" << num + num_Gift << oneComm.m_Unit << "��";
		cout << "���ۣ�" << fixed << setprecision(2) << oneComm.m_PerPrice << "(Ԫ)" << "��";
		if (num_Gift != 0)//���������һ
		{
			GiftComm.insert(map<string, int>::value_type(obj.first, num_Gift));
			cout << "С�ƣ�" << oneComm.m_PerPrice*obj.second << "(Ԫ)";
			totalPrice += oneComm.m_PerPrice*obj.second;
			saveMoney += oneComm.m_PerPrice*num_Gift;
		}
		else
		{
			cout << "С�ƣ�" << oneComm.m_PerPrice*num*oneComm.m_Count << "(Ԫ)";
			totalPrice += oneComm.m_PerPrice*num*(1 - oneComm.m_Count);
			//�����ۿ�
			if (oneComm.m_Count < 1.0)
			{
				cout << "��" << "��ʡ" << oneComm.m_PerPrice*num*oneComm.m_Count << "(Ԫ)";
				saveMoney += oneComm.m_PerPrice*num*oneComm.m_Count;
			}
		}
		cout << endl;
	}
	cout << "----------------------" << endl;
	if (!GiftComm.empty())
	{
		cout << "�����һ��Ʒ��" << endl;
		for (auto obj : GiftComm)
		{
			CommodityClass& oneComm = CommTable[obj.first];
			int num = obj.second;
			cout << "���ƣ�" << oneComm.m_Name << "��" << "������" << num << oneComm.m_Unit << endl;
		}
		cout << "----------------------" << endl;

	}
	cout << "�ܼƣ�" << totalPrice << "Ԫ" << endl;
	if (saveMoney > 0.0)
	{
		cout << "��ʡ��" << saveMoney << "Ԫ" << endl;
	}
	cout << "**********************" << endl;
}
int main()
{
	//ûǮ׬�̵��������Ʒ����
	CommodityClass one;
	{
		one.m_Gift = 0.5;
		one.m_Count = 1.0;
		one.m_Name = "�ɿڿ���";
		one.m_PerPrice = 2.0;
		one.m_Type = "����";
		one.m_Unit = "ƿ";
		CommTable["ITEM000001"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 0.9;
		one.m_Name = "ƻ��";
		one.m_PerPrice = 5.5;
		one.m_Type = "ˮ��";
		one.m_Unit = "��";
		CommTable["ITEM000002"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 1.0;
		one.m_Name = "��ë��";
		one.m_PerPrice = 1.0;
		one.m_Type = "����";
		one.m_Unit = "��";
		CommTable["ITEM000003"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 1.0;
		one.m_Name = "���¿���";
		one.m_PerPrice = 2.0;
		one.m_Type = "����";
		one.m_Unit = "ƿ";
		CommTable["ITEM000004"] = one;
	}
	{
		one.m_Gift = 0.0;
		one.m_Count = 1.0;
		one.m_Name = "�Ӷ౦";
		one.m_PerPrice = 3.0;
		one.m_Type = "����";
		one.m_Unit = "ƿ";
		CommTable["ITEM000005"] = one;
	}
	vector<string> Input;
	GetInput(Input);
	SortInput(Input);
	PrintOut(InputData);
	return true;
}