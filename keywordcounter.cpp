#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<string.h>
#include<map>
#include<set>
#include<stdlib.h>
using namespace std;
struct node
{
	int data;
	int degree;
	node* child;
	node* parent;
	node* left;
	node* right;
	bool childcut;
};
class Fib_heap
{
private:
	node* max_node;
public:
	Fib_heap()
	{
		max_node = NULL;
	}
	node* initialize_heap();
	node* create_node(int data)
	{
		node* a = new node();
		a->data = data;

		a->degree = 0;
		a->parent = NULL;
		a->child = NULL;
		a->right = a;
		a->left = a;
		a->childcut = false;
		return a;
	}

	node* RemoveMax()
	{
		//Make parent pointer null for all children
		//To remove the node with maximum frequency
		node* p = max_node->child;
		if (NULL != p)
		{
			
			do {
				p->parent = NULL;
				p = p->right;
			} while (p != max_node->child && p != p->right);
		}
		//Disconnect child from parent and add them to heap
		if (max_node->child != NULL && max_node->right != max_node)
		{
			
			max_node->child->left->right = max_node->right;
			max_node->right->left = max_node->child->left;
			max_node->child->left = max_node;
			max_node->right = max_node->child;
			max_node->degree = 0;
			max_node->child = NULL;
		}
		else {
			if (max_node->child != NULL)
			{
				
				max_node->child->left->right = max_node->right;
				max_node->right->left = max_node->child->left;
				max_node->child->left = max_node;
				max_node->right= max_node->child;
				max_node->degree = 0;
				max_node->child = NULL;
			}
		}
		p = max_node->right;
		node* m;
		node* n;
		node* max2 = NULL;
		map<int, node*> a;
		map<int, node*>::iterator lt;
		int maxval = -999;
		vector<node*> tmp;
		tmp.clear();
		while (p != max_node)
		{
			tmp.push_back(p);
			p = p->right;
		}
		for (int i = 0; i < tmp.size(); i++)
		{
			tmp[i]->left = tmp[i];
			tmp[i]->right = tmp[i];
		}
		//Traverse Heap 
		for(int i=0;i<tmp.size();i++)
		{
			
			lt = a.find(tmp[i]->degree);
			while (lt != a.end())
			{
				//Pairwise combine nodes with respect to degree
				if (tmp[i]->data > lt->second->data)
				{
					n = tmp[i];
					m = lt->second;
					m->left->right = m->right;
					m->right->left = m->left;
					if (n->child == NULL)
					{


						m->right = m;
						m->left = m;
						n->child = m;
						m->parent = n;

						n->degree = n->degree + 1;
					}
					else
					{
						m->left = n->child;
						n->child->right->left = m;
						m->right = n->child->right;
						n->child->right = m;
						m->parent = n;
						n->degree = n->degree + 1;
					}
					

				}
				else {
					m = tmp[i];
					n = lt->second;
					
					
					if (n->child == NULL)
					{


						m->right = m;
						m->left = m;
						n->child = m;
						m->parent = n;

						n->degree = n->degree + 1;
					}
					else
					{
						m->left = n->child;
						n->child->right->left = m;
						m->right = n->child->right;
						n->child->right = m;
						m->parent = n;
						n->degree = n->degree + 1;
					}
					tmp[i] = n;
				}

				a.erase(lt);
				lt = a.find(tmp[i]->degree);
			}
			if (lt == a.end()) {
				
				a.insert({ tmp[i]->degree, tmp[i] });
			}
			
		}
		map<int, node*>::iterator it;
		it = a.begin();
		it->second->right = it->second;
		it->second->left = it->second;
		max2 = it->second;
	     while(it != a.end())
		{
			
			 max2 = meld(max2, it->second);
			 it++;
		}

		
		node*tmp1;
		tmp1 = max_node;
		max_node = max2;
		return tmp1;
	}
	node* meld(node *p, node *q)
	//To join the two lists into one
	{
		
		if (p->right == p)
		{
			p->left = q;
			q->left = p;
			p->right = q;
			q->right = p;
		}
		else
		{
			p->right->left = q;
			q->right = p->right;
			p->right = q;
			q->left = p;
		}
		if (q->data > p->data)
		{
			return q;
		}
		return p;
	}
	void IncreaseKey(node* p, int data)
	{
		//To increase the value of the node
		p->data = p->data + data;
		if (p->parent == NULL)
		{
			if (max_node->data < p->data)
			{
				max_node = p;
			}
			return;
		}
		if (p->data > p->parent->data)
		{
			node* l;
			do
			{
				if (p->right == p)
				{
					p->parent->child = NULL;
				}
				else
				{
					p->parent->child = p->right;
					p->left->right = p->right;
					p->right->left = p->left;
				}
				p->parent->degree = p->parent->degree - 1;
				max_node->right->left = p;
				p->right = max_node->right;
				max_node->right = p;
				p->left = max_node;
				p->parent->childcut = true;
				if (max_node->data < p->data)
				{
					max_node = p;
				}
				l = p->parent;
				p->parent = NULL;
				p = l;
			} while (p->parent != NULL && p->parent->childcut == true);
		}
	}
	node* insert(int data)
	{
		//Inserting data into new node
		node *p = create_node(data);
		if (max_node == NULL)
		{
			max_node = p;
			return p;
		}
		else
		{
			if (max_node->left == max_node) {
				max_node->left = p;
				p->right = max_node;
				p->left = max_node;
				max_node->right = p;
			}
			else {
				max_node->right->left = p;
				p->right = max_node->right;
				p->left = max_node;
				max_node->right = p;

			}
			if (p->data > max_node->data)
			{
				max_node = p;
			}

		}
		return p;
	}
	void printnode()
	{
		//To print the node
		node*p = max_node;
		do
		{
			cout << p->data << "\n";
			p = p->right;
		} while (p != max_node);
	}
};

int main(int c, char *v[])
{

       if(c == 2)
       {
	Fib_heap* p = new Fib_heap();
	
//To get the data from input file and display it in output file
	ifstream file(v[1]);
	ofstream outfile;
	outfile.open("output_file.txt");
	string line;
	string str, comp;
	int k = 0;
	map<string, node*> keywords;
	map<string, node*>::iterator iter;
	map<node*, string> rev_keys;
	map<node*, string> ::iterator iter2;
	vector<pair<string, node*>> tmp;
	while (file)
	{
		k = 0;
		if (!getline(file, line))
		{
			break;
		}
		istringstream line2(line);
		while (line2)
		{

			if ((!getline(line2, str, ' ')))
			{
				break;
			}

			if (str[0] == '$')
			{
				comp = str.substr(1, str.size() - 1);
				k = 1;
			}
			else if (k == 1)
			{
				int data = atoi(str.c_str());
				iter = keywords.find(comp);
				if (iter == keywords.end())
				{
					node* q;
					
					q = p->insert(data);
					keywords.insert({ comp,q });
					rev_keys.insert({ q,comp });
				}
				else
				{
					
					p->IncreaseKey(iter->second, data);
				}
			}
			else if (str == "stop")
			{
				outfile.close();
		 		exit(1);
			}
			else
			{
				outfile << "\n";

				int h = atoi(str.c_str());


				for (int i = 0; i < h; i++)
				{
					node* q = p->RemoveMax();

					iter2 = rev_keys.find(q);
					//outfile << iter2->second << "\t" << iter2->first->data << "\n";
					outfile << iter2->second;
					if(i != h-1)
					{
					outfile<<",";
					}
					tmp.push_back(make_pair(iter2->second, iter2->first));
				}
				for (int j = 0; j < tmp.size(); j++)
				{
					keywords.erase(tmp[j].first);
					rev_keys.erase(tmp[j].second);
					node*z;
					z = p->insert(tmp[j].second->data);
					keywords.insert({ tmp[j].first,z });
					rev_keys.insert({ z,tmp[j].first });
				}
				if (h == 1)
				{
					int g = 0;
				}
				tmp.clear();
			}
		}

	}
file.close();
}
else
{
cout<<"Invalid arguments";
}
	return 0;	
}
