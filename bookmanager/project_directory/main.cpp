
#include "operation.h"
#include "BOOK.h"
#include"log_ctl.h"
#include<fstream>
MYSQL sql;
operation manager;
int isLog;

int login() {
	cout << "输入用户id和密码：" << endl;
	Manager m;
	int x1; 
	bool is1;
	std::string password;
	cin >> m.id >> password;
	if (m.id.empty() || password.empty())
	{
		cout << "输入错误";
	}
	else
	{
		is1 = manager.selectManager(m, sql);
		if (is1 == 0) x1 = 0;	//用户不存在
		else if (m.password != password) x1 = 1;	//密码错误
		else x1 = 2;
		if (x1 == 0) cout << "用户不存在";
		else if (x1 == 1) cout << "密码错误";
		else isLog = 1;
	}
	return x1;
}

int main()
{	
	cout << "输入主机，用户名，密码，数据库名称" << endl;
	string host, username, password, db_name;
	cin >> host >> username >> password >> db_name;
    log_ctl log(host,username,password,db_name);
    if (log.log(sql)) {
        cout << "已成功连接到数据库"<<endl;
    }
    else {
        cout << "连接数据库失败"<<endl;
        return 0;
    }
    cout << "*************************************************************************\n\t\t\t\t浙江大学图书馆\n*************************************************************************\n";
    while (1) {
        cout << "1.图书查询 2.借书 3.还书 4.图书管理 5.借阅证管理 6.信息查询 0.退出系统" << endl;
        int choice=0;
		cin >> choice;
        switch (choice) {
		case 0: {
			mysql_close(&sql);
			exit(0);
		}
		case 1: {
			BookQuery q;
			cout << "输入图书信息：id，书名，作者，种类，最早年份，最晚年份，出版社，最低价格，最高价格，是否有余量（1 或 0 ）（空信息输入0）" << endl;
			cin >> q.book_id >> q.name >> q.author >> q.type >> q.begin_year >> q.end_year >> q.publisher >> q.low_price >> q.high_price >> q.have_stock;
			manager.select(q, sql);
			if (q.Catalog.empty()) cout << "查找失败" << endl;
			else {
				cout << "id，书名，作者，价钱，总量，余量，出版社，出版日期，类型" << endl;
				for (int i = 0; i < q.Catalog.size(); ++i) {
					cout << q.Catalog[i].book_id << ' ' << q.Catalog[i].name << ' ' << q.Catalog[i].author << ' ' << q.Catalog[i].price << ' ' << q.Catalog[i].num << ' ' << q.Catalog[i].stock << ' ' << q.Catalog[i].publisher << ' ' << q.Catalog[i].publish_year << ' ' << q.Catalog[i].type << endl;
				}
			}
			break;
		}
		case 2: {
			if (!isLog) login();
			if (!isLog) {
					cout << "登陆失败！" << endl;
					break;
			}
				else {
					cout << "输入用户id，图书id" << endl;
					string userid, bookid;
					cin >> userid >> bookid;
					int borrow_state = manager.borrowbook(userid, bookid, sql);
					switch (borrow_state) {
					case 0: cout << "没有库存！"<<endl;
						break;
					case 1: cout << "借书失败！" << endl;
						break;
					case 2: cout << "恢复余量失败！" << endl;
						break;
					case 3: cout << "借书成功！" << endl;
						break;
					case 4:cout << "查询图书失败！" << endl;
						break;
					default:
						break;
					}
					break;
				}
			
		case 3: {
			if (!isLog) login();
				if (!isLog) {
					cout << "登陆失败！" << endl;
					break;
				}
				else {
					cout << "输入用户id，图书id" << endl;
					string userid, bookid;
					cin >> userid >> bookid;
					if (manager.returnbook(userid, bookid, sql)) {
						cout << "还书成功" << endl;
					}
					else {
						cout << "还书失败" << endl;
					}
				}
				break;
		}
		case 4: {
			if (!isLog) login();
			if(!isLog) {
				cout << "登陆失败！" << endl;
				break;
			}
			else {
				cout << "1.新增图书 2.删除图书 3.批量入库" << endl;
				int op;
				cin >> op;
				switch (op) {
				case 1: {
					cout << "输入id，书名，作者，价钱，总量，余量，出版社，出版日期，类型" << endl;
					Book b;
					cin >>b.book_id  >>b.name  >>b.author  >>b.price  >>b.num  >>b.stock  >>b.publisher  >>b.publish_year  >>b.type ;
					int fail=manager.insert(b, sql);
					if (fail) {
						cout << "插入失败！" << endl;
					}
					else {
						cout << "插入成功！" << endl;
					}
					break;
				}
				case 2: {
					cout << "输入图书id" << endl;
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
						cout << "插入失败！" << endl;
					}
					else {
						cout << "插入成功！" << endl;
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
				cout << "登陆失败！" << endl;
				break;
			}
			else {
				cout << "1.添加新用户 2.删除用户 3.查询用户信息" << endl;
				User u;
				int op;
				cin >> op;
				switch (op)
				{
				case 1: {
					cout << "输入id，用户名，密码"<<endl;
					cin >> u.id >> u.name >> u.password;
					u.booknum = 0;
					int success=manager.insertuser(u, sql);
					if (success == 1) cout << "新建成功！" << endl;
					else cout << "新建失败！" << endl;
					break;
				}
				case 2: {
					cout << "输入id" << endl;
					cin >> u.id;
					int success = manager.deleteuser(u, sql);
					if (success == 1) cout << "删除成功！" << endl;
					else cout << "删除失败！" << endl;
					break;
				}
				case 3: {
					cout << "输入id" << endl;
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
				cout << "登陆失败！" << endl;
				break;
			}
			cout << "1.借书状况查询 2.用户借书信息查询 " << endl;
			int op;
			cin >> op;
			if (op == 1) {
				UserQuery u;
				int success = manager.displayUser(u, sql);
				if (!success) {
					cout << "查询失败！" << endl;
					break;
				}
				for (int i = 0; i < u.Users.size(); ++i) {
					cout << u.Users[i].id << ' ' << u.Users[i].name << ' ' << u.Users[i].booknum << endl;
				}
			}
			else if (op == 2) {
				BookQuery q;
				string userid;
				cout << "输入用户id：" << endl;
				cin >> userid;
				int success = manager.displayborrow(userid, q, sql);
				if (!success) {
					cout << "查询失败！" << endl;
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
