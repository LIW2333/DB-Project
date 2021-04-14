
#include "operation.h"
#include "BOOK.h"
#include"log_ctl.h"
#include<fstream>
MYSQL sql;
operation manager;
int isLog;

int login() {
	cout << "�����û�id�����룺" << endl;
	Manager m;
	int x1; 
	bool is1;
	std::string password;
	cin >> m.id >> password;
	if (m.id.empty() || password.empty())
	{
		cout << "�������";
	}
	else
	{
		is1 = manager.selectManager(m, sql);
		if (is1 == 0) x1 = 0;	//�û�������
		else if (m.password != password) x1 = 1;	//�������
		else x1 = 2;
		if (x1 == 0) cout << "�û�������";
		else if (x1 == 1) cout << "�������";
		else isLog = 1;
	}
	return x1;
}

int main()
{	
	cout << "�����������û��������룬���ݿ�����" << endl;
	string host, username, password, db_name;
	cin >> host >> username >> password >> db_name;
    log_ctl log(host,username,password,db_name);
    if (log.log(sql)) {
        cout << "�ѳɹ����ӵ����ݿ�"<<endl;
    }
    else {
        cout << "�������ݿ�ʧ��"<<endl;
        return 0;
    }
    cout << "*************************************************************************\n\t\t\t\t�㽭��ѧͼ���\n*************************************************************************\n";
    while (1) {
        cout << "1.ͼ���ѯ 2.���� 3.���� 4.ͼ����� 5.����֤���� 6.��Ϣ��ѯ 0.�˳�ϵͳ" << endl;
        int choice=0;
		cin >> choice;
        switch (choice) {
		case 0: {
			mysql_close(&sql);
			exit(0);
		}
		case 1: {
			BookQuery q;
			cout << "����ͼ����Ϣ��id�����������ߣ����࣬������ݣ�������ݣ������磬��ͼ۸���߼۸��Ƿ���������1 �� 0 ��������Ϣ����0��" << endl;
			cin >> q.book_id >> q.name >> q.author >> q.type >> q.begin_year >> q.end_year >> q.publisher >> q.low_price >> q.high_price >> q.have_stock;
			manager.select(q, sql);
			if (q.Catalog.empty()) cout << "����ʧ��" << endl;
			else {
				cout << "id�����������ߣ���Ǯ�������������������磬�������ڣ�����" << endl;
				for (int i = 0; i < q.Catalog.size(); ++i) {
					cout << q.Catalog[i].book_id << ' ' << q.Catalog[i].name << ' ' << q.Catalog[i].author << ' ' << q.Catalog[i].price << ' ' << q.Catalog[i].num << ' ' << q.Catalog[i].stock << ' ' << q.Catalog[i].publisher << ' ' << q.Catalog[i].publish_year << ' ' << q.Catalog[i].type << endl;
				}
			}
			break;
		}
		case 2: {
			if (!isLog) login();
			if (!isLog) {
					cout << "��½ʧ�ܣ�" << endl;
					break;
			}
				else {
					cout << "�����û�id��ͼ��id" << endl;
					string userid, bookid;
					cin >> userid >> bookid;
					int borrow_state = manager.borrowbook(userid, bookid, sql);
					switch (borrow_state) {
					case 0: cout << "û�п�棡"<<endl;
						break;
					case 1: cout << "����ʧ�ܣ�" << endl;
						break;
					case 2: cout << "�ָ�����ʧ�ܣ�" << endl;
						break;
					case 3: cout << "����ɹ���" << endl;
						break;
					case 4:cout << "��ѯͼ��ʧ�ܣ�" << endl;
						break;
					default:
						break;
					}
					break;
				}
			
		case 3: {
			if (!isLog) login();
				if (!isLog) {
					cout << "��½ʧ�ܣ�" << endl;
					break;
				}
				else {
					cout << "�����û�id��ͼ��id" << endl;
					string userid, bookid;
					cin >> userid >> bookid;
					if (manager.returnbook(userid, bookid, sql)) {
						cout << "����ɹ�" << endl;
					}
					else {
						cout << "����ʧ��" << endl;
					}
				}
				break;
		}
		case 4: {
			if (!isLog) login();
			if(!isLog) {
				cout << "��½ʧ�ܣ�" << endl;
				break;
			}
			else {
				cout << "1.����ͼ�� 2.ɾ��ͼ�� 3.�������" << endl;
				int op;
				cin >> op;
				switch (op) {
				case 1: {
					cout << "����id�����������ߣ���Ǯ�������������������磬�������ڣ�����" << endl;
					Book b;
					cin >>b.book_id  >>b.name  >>b.author  >>b.price  >>b.num  >>b.stock  >>b.publisher  >>b.publish_year  >>b.type ;
					int fail=manager.insert(b, sql);
					if (fail) {
						cout << "����ʧ�ܣ�" << endl;
					}
					else {
						cout << "����ɹ���" << endl;
					}
					break;
				}
				case 2: {
					cout << "����ͼ��id" << endl;
					string bookid;
					cin >> bookid;
					manager.deletebook(bookid, sql);
					break;
				}
				case 3: {
					ifstream fout;
					fout.open("C:\\Users\\bucket\\source\\repos\\library\\data.txt");
					int n;
					fout >> n;
					//cout << n<<endl;
					Book b;
					int fail=0;
					for (int i = 0; i < n; ++i) {
						fout >> b.book_id >> b.name >> b.author >> b.price >> b.num >> b.stock >> b.publisher >> b.publish_year >> b.type;
						//cout<< b.book_id << b.name << b.author << b.price << b.num << b.stock << b.publisher << b.publish_year << b.type<<endl;
						if (!fail) {
							fail = manager.insert(b, sql);
						}
					}
					if (fail) {
						cout << "����ʧ�ܣ�" << endl;
					}
					else {
						cout << "����ɹ���" << endl;
					}
					fout.close();
					break;
				}
				default:
					break;
				}
				break;
			}
		case 5: {
			if (!isLog) login();
			if (!isLog) {
				cout << "��½ʧ�ܣ�" << endl;
				break;
			}
			else {
				cout << "1.������û� 2.ɾ���û� 3.��ѯ�û���Ϣ" << endl;
				User u;
				int op;
				cin >> op;
				switch (op)
				{
				case 1: {
					cout << "����id���û���������"<<endl;
					cin >> u.id >> u.name >> u.password;
					u.booknum = 0;
					int success=manager.insertuser(u, sql);
					if (success == 1) cout << "�½��ɹ���" << endl;
					else cout << "�½�ʧ�ܣ�" << endl;
					break;
				}
				case 2: {
					cout << "����id" << endl;
					cin >> u.id;
					int success = manager.deleteuser(u, sql);
					if (success == 1) cout << "ɾ���ɹ���" << endl;
					else cout << "ɾ��ʧ�ܣ�" << endl;
					break;
				}
				case 3: {
					cout << "����id" << endl;
					cin >> u.id;
					int success = manager.selectUser(u, sql);
					cout << u.id <<' '<< u.name <<' '<< u.password<<endl;
					break;
				}
				default:
					break;
				}
			}
			break;
		}
		case 6: {
			if (!isLog) login();
			if (!isLog) {
				cout << "��½ʧ�ܣ�" << endl;
				break;
			}
			cout << "1.����״����ѯ 2.�û�������Ϣ��ѯ " << endl;
			int op;
			cin >> op;
			if (op == 1) {
				UserQuery u;
				int success = manager.displayUser(u, sql);
				if (!success) {
					cout << "��ѯʧ�ܣ�" << endl;
					break;
				}
				for (int i = 0; i < u.Users.size(); ++i) {
					cout << u.Users[i].id << ' ' << u.Users[i].name << ' ' << u.Users[i].booknum << endl;
				}
			}
			else if (op == 2) {
				BookQuery q;
				string userid;
				cout << "�����û�id��" << endl;
				cin >> userid;
				int success = manager.displayborrow(userid, q, sql);
				if (!success) {
					cout << "��ѯʧ�ܣ�" << endl;
					break;
				}
				else {
					for (int i = 0; i < q.Catalog.size(); ++i) {
						cout << q.Catalog[i].book_id << ' ' << q.Catalog[i].name << ' ' << q.Catalog[i].author << ' ' << q.Catalog[i].price << ' ' << q.Catalog[i].num << ' ' << q.Catalog[i].stock << ' ' << q.Catalog[i].publisher << ' ' << q.Catalog[i].publish_year << ' ' << q.Catalog[i].type << endl;
					}
				}
			}
			break;
		}
		default:
			break;
		}


		}

        }
    }

    system("pause");
}
