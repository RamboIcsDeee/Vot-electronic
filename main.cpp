#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
using namespace std;

struct Date {
    string nume;
    string adresa;
    string cnp;
    string serie;
    bool votat = false;
    Date(string arg_nume, string arg_adresa, string arg_cnp, string arg_serie)
    {
        nume = arg_nume;
        adresa = arg_adresa;
        cnp = arg_cnp;
        serie = arg_serie;
    }
};
void completare_database(vector<Date>& votanti)
{
    ifstream fin("evidenta.csv");
    string line;
    while (getline(fin, line))
    {
        string nume, adresa, cnp, serie;
        istringstream iss(line);

        getline(iss, nume, ',');
        getline(iss, adresa, ',');
        getline(iss, cnp, ',');
        getline(iss, serie, '\n');

        struct Date votant(nume, adresa, cnp, serie);
        votanti.push_back(votant);
    }
}
bool verificare_cnp(string cnp)
{
    if (cnp[0] == '1' || cnp[0] == '2')
    {
        return true;
    }
    if (cnp[0] == '5' || cnp[0] == '6')
    {
        return true;
    }
    return false;
}
vector<Date>::iterator get_id(vector<Date>& votanti, string cnp)
{
    auto it = votanti.begin();
    for (it = votanti.begin(); it != votanti.end(); it++)
    {
        if (it->cnp == cnp)
        {
            break;
        }
    }
    return it;
}
bool check_votat(string cnp, vector<Date>& votanti)
{
    auto it = get_id(votanti, cnp);
    if (it->cnp == cnp)
    {
        if (it->votat)
        {
            return true;
        }
    }
    return false;
}
bool exist(string cnp, vector<Date>& votanti)
{
    bool ok = false;
    auto it = get_id(votanti, cnp);
    if (it->cnp == cnp)
    {
        ok = true;
    }
    return ok;
}
bool major(string cnp)
{
    int luna = 0;

    if (cnp[0] == '1' || cnp[0] == '2')
    {
        return true;
    }
    if (cnp[0] == '5' || cnp[0] == '6')
    {
        if (cnp[1] == '0' && cnp[2] == '0')
        {
            luna = (cnp[3] - 48) * 10 + (cnp[4] - 48);
            if (luna <= 5)
            {
                return true;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}
void statistics(int fraude, int total, map<int, pair<string, int>>& voturi)
{
    cout << "Statistici" << endl;
    cout << "==========" << endl;
    for (auto it = voturi.begin(); it != voturi.end(); it++)
    {
        double rezultat = (it->second.second * 1.00 / total) * 100.00;
        cout << (*it).second.first << " : " << (*it).second.second << " voturi (" << setprecision(2) << fixed << rezultat << "%)" << endl;
    }
    cout << "Incercari de frauda: " << fraude << endl;
}
void voting(vector<Date>& votanti, map<int, pair<string, int>>& voturi)
{
    string optiune, cnp, votat;
    string line;
    int fraude = 0;
    int total = 0;
    while (cin >> optiune)
    {
        if (optiune == "+")
        {
            getchar();
            getline(cin, line);
            istringstream iss(line);
            getline(iss, cnp, ' ');
            getline(iss, votat, '\n');
            if (verificare_cnp(cnp))
            {
                if (exist(cnp, votanti))
                {
                    if (major(cnp))
                    {
                        if (!check_votat(cnp, votanti))
                        {
                            auto itr = get_id(votanti, cnp);
                            for (auto it = voturi.begin(); it != voturi.end(); it++)
                            {

                                if (it->second.first == votat)
                                {
                                    it->second.second++;
                                }
                                else
                                {
                                    voturi.insert({ (int)voturi.size(),{votat,1} });
                                    break;
                                }
                            }
                            if (voturi.empty())
                            {
                                voturi.insert({ (int)voturi.size(),{votat,1} });
                            }
                            (*itr).votat = true;
                            total++;
                        }
                        else
                        {
                            cout << "Vot deja inregistrat" << endl;
                            fraude++;
                        }
                    }
                    else cout << "Persoana minora" << endl;
                }
                else
                {
                    cout << "CNP invalid" << endl;
                    fraude++;
                }
            }
            else cout << "CNP invalid" << endl;
        }
        if (optiune == "?")
        {
            statistics(fraude, total, voturi);
        }
        if (optiune == "*")
        {
            statistics(fraude, total, voturi);
            break;
        }
    }
}
int main()
{
    vector<Date>* votanti = new vector<Date>;
    map<int,pair<string,int>>* voturi = new map<int, pair<string, int>>;
    completare_database(*votanti);
    voting(*votanti, *voturi);
    return 0;
}
