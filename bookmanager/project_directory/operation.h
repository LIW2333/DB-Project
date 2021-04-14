#ifndef _MANAGER_H_
#define _MANAGER_H_
#include"BOOK.h"
#include"mysql.h"
#include"User.h"
#include<string>
#include<stdio.h>
#include<iostream>
#include<time.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libmysql.lib")

class operation {
private:
	string interpret(BookQuery & q) {
		if ((q.book_id=="0") && (q.name=="0") && (q.author=="0") && (q.publisher=="0") && (!q.begin_year)
			&& (!q.end_year) && (q.low_price < 0) && (q.high_price < 0) && (q.type=="0"))
		{
			if (q.have_stock)
				return "select * from book where stock > 0;";
			else return "select * from book;";
		}
		string query = "select * from book where ";

		if (q.book_id!="0") {
			query += " book_id= '" + q.book_id + "' and";
		}
		if (q.name!="0") {
			query += " name= '" + q.name + "' and";
		}
		if (q.author!="0") {
			query += " author_name = '" + q.author + "' and";
		}
		if (q.publisher!="0") {
			query += " publisher= '" + q.publisher + "' and";
		}
		if (q.begin_year) {
			query += " publish_year>= " + to_string(q.begin_year) + " and";
		}
		if (q.end_year) {
			query += " publish_year<= " + to_string(q.end_year)+ " and";
		}
		if (q.low_price) {
			query += " price >= " + to_string(q.low_price) + " and";
		}
		if (q.high_price > 0 && q.high_price >= q.low_price) {
			query += " price <= " + to_string(q.high_price) + " and";
		}
		if (q.have_stock) {
			query += " stock > 0 and";
		}
		if (q.type!="0") {
			query += " type = '" + q.type + "' and";
		}
		query.erase(query.size() - 3, 3);
		query += ";";
		return query;
	}
	
	int type;
public:
	
	void select(BookQuery & q, MYSQL& sql) {
		q.Catalog.clear();
		MYSQL_RES *res_ptr;
		MYSQL_ROW result_row;
		Book tem;
		int column, row;
		string query = interpret(q);
		if (query.size() > 0)
		{
			mysql_query(&sql, query.c_str());
			res_ptr = mysql_store_result(&sql);
			column = mysql_num_fields(res_ptr);
			row = mysql_num_rows(res_ptr) + 1;
			for (int i = 1; i < row; i++) {
				result_row = mysql_fetch_row(res_ptr);
				tem.book_id = result_row[0];
				tem.name = result_row[1];
				tem.author = result_row[2];
				tem.price = atof(result_row[3]);
				tem.num = atoi(result_row[4]);
				tem.stock = atoi(result_row[5]);
				tem.publisher = result_row[6];
				tem.publish_year = result_row[7];
				tem.type = result_row[8];
				q.Catalog.push_back(tem);
			}
		}
	}
	
	int insert(Book& q, MYSQL& sql) {
		MYSQL_RES *res_ptr;
		MYSQL_ROW result_row;
		string query = "select * from book where book_id ='" + q.book_id + "';";
		mysql_query(&sql, query.c_str());
		res_ptr = mysql_store_result(&sql);
		result_row = mysql_fetch_row(res_ptr);
		//row = mysql_num_rows(res_ptr);
		if (result_row == NULL) {
			query = "insert into book values('";
			query += q.book_id + "','" + q.name + "','" + q.author + "'," + to_string(q.price);

			query += "," + to_string(q.num) + ",";
			query += to_string(q.stock) + ",'" + q.publisher + "','";
			query += q.publish_year + "','" + q.type + "');";
			int x = mysql_query(&sql, query.c_str());
			return x;
		}
		else {
			int num = atoi(result_row[4]);
			int stock = atoi(result_row[5]);
			int x = 0;
			if (q.name != result_row[1] && !x) {
				string query = "UPDATE book SET name='" + q.name + "' where book_id='" + q.book_id + "';";
				x = mysql_query(&sql, query.c_str());
			}
			if (q.author != result_row[2] && !x) {
				string query = "UPDATE book SET author_name='" + q.author + "' where book_id='" + q.book_id + "';";
				x = mysql_query(&sql, query.c_str());
			}
			if (q.price != atoi(result_row[3]) && !x) {
				string query = "UPDATE book SET price='" + to_string(q.price) + "' where book_id='" + q.book_id + "';";
				x = mysql_query(&sql, query.c_str());
			}
			if (q.publisher != result_row[6] && !x) {
				string query = "UPDATE book SET publisher='" + q.publisher + "' where book_id='" + q.book_id + "';";
				x = mysql_query(&sql, query.c_str());
			}
			if (q.publish_year != result_row[7] && !x) {
				string query = "UPDATE book SET publisher='" + q.publish_year + "' where book_id='" + q.book_id + "';";
				x = mysql_query(&sql, query.c_str());
			}
			if (q.type != result_row[8] && !x) {
				string query = "UPDATE book SET publisher='" + q.type + "' where book_id='" + q.book_id + "';";
				x = mysql_query(&sql, query.c_str());
			}
			query = "UPDATE book SET num = num+" + to_string(q.num) + " ,stock=stock+" + to_string(q.stock) + " WHERE book_id = '" + q.book_id + "' ;";
			if(!x) x = mysql_query(&sql, query.c_str());
			return x;
		}
	}
	bool insertuser(User & _user, MYSQL &sql) {
		string query;
		query = "insert into user values('" + _user.id + "','" + _user.name + "','" + _user.password + "');";
		int r = mysql_query(&sql, query.c_str());
		return r == 0;
	}
	bool selectUser(User &_user, MYSQL &sql) {
		string query, query2;
		MYSQL_RES *res_ptr, *res_ptr1;
		MYSQL_ROW result_row, result_row1;
		int row;
		query = "select * from user where id='" + _user.id + "';";
		query2 = "select count(*) from borrow where user_id ='" + _user.id + "';";
		mysql_query(&sql, query.c_str());
		res_ptr = mysql_store_result(&sql);
		row = mysql_num_rows(res_ptr);
		mysql_query(&sql, query2.c_str());
		res_ptr1 = mysql_store_result(&sql);
		result_row1 = mysql_fetch_row(res_ptr1);
		_user.booknum = atoi(result_row1[0]);
		if (row == NULL) return 0;
		else {
			result_row = mysql_fetch_row(res_ptr);
			_user.name = result_row[1];
			_user.password = result_row[2];
			return 1;
		}
	}
	bool selectManager(Manager &_manager, MYSQL &sql) {
		string query;
		MYSQL_RES *res_ptr;
		MYSQL_ROW result_row;
		int row;
		query = "select * from manager where id='" + _manager.id + "';";
		mysql_query(&sql, query.c_str());
		res_ptr = mysql_store_result(&sql);
		row = mysql_num_rows(res_ptr);
		if (row == NULL) return 0;
		else {
			result_row = mysql_fetch_row(res_ptr);
			_manager.name = result_row[1];
			_manager.password = result_row[2];
			return 1;
		}
	}

	bool displayUser(UserQuery &q, MYSQL &sql) {
		string query, query2;
		MYSQL_RES *res_ptr, *res_ptr1;
		MYSQL_ROW result_row, result_row1;
		int row;
		query = "select * from user;";
		query2 = "select count(*) from borrow where user_id ='";
		int r = mysql_query(&sql, query.c_str());
		if (r != 0)return 0;
		else 
		{
			res_ptr = mysql_store_result(&sql);
			row = mysql_num_rows(res_ptr) + 1;
			User tem;
			for (int i = 1; i < row; i++) {
				result_row = mysql_fetch_row(res_ptr);
				tem.id = result_row[0];
				tem.name = result_row[1];
				tem.password = result_row[2];
				query2 = "select count(*) from borrow where user_id ='";
				query2 += tem.id + "';";
				mysql_query(&sql, query2.c_str());
				res_ptr1 = mysql_store_result(&sql);
				result_row1 = mysql_fetch_row(res_ptr1);
				tem.booknum = atoi(result_row1[0]);
				q.Users.push_back(tem);//将查询信息压到vector中，主要查询信息为借书数量
			}
			return true;
		}
	}
	int displayborrow(string user_id, BookQuery &q, MYSQL& sql) {
		string query;
		MYSQL_RES *res_ptr, *res_ptr1;
		MYSQL_ROW result_row;
		q.Catalog.clear();
		query = "select * from user where id='" + user_id + "';";
		mysql_query(&sql, query.c_str());
		res_ptr = mysql_store_result(&sql);
		result_row = mysql_fetch_row(res_ptr);
		if (result_row == NULL) {
			q.Catalog.clear();
			return 0;
		}
		query = "select book_id from borrow where user_id='" + user_id + "'";
		mysql_query(&sql, query.c_str());
		res_ptr = mysql_store_result(&sql);
		int  row;
		row = mysql_num_rows(res_ptr);
		for (int i = 1; i < row + 1; i++) {
			result_row = mysql_fetch_row(res_ptr);
			string book_id = result_row[0];
			query = "select * from book where book_id='" + book_id + "'";
			mysql_query(&sql, query.c_str());
			res_ptr1 = mysql_store_result(&sql);
			result_row = mysql_fetch_row(res_ptr1);
			Book tem;
			tem.book_id = result_row[0];
			tem.name = result_row[1];
			tem.author = result_row[2];
			tem.price = atof(result_row[3]);
			tem.num = atoi(result_row[4]);
			tem.stock = atoi(result_row[5]);
			tem.publisher = result_row[6];
			tem.publish_year = result_row[7];
			tem.type = result_row[8];
			q.Catalog.push_back(tem);
		}
		return 1;
	}

	int borrowbook(string user_id, string book_id, MYSQL &sql) { //user manager
		MYSQL_RES *res_ptr;
		MYSQL_ROW result_row;
		int y;
		string query = "select * from book where book_id ='" + book_id + "';";
		y = mysql_query(&sql, query.c_str());
		if (y) return 4;
		res_ptr = mysql_store_result(&sql);
		result_row = mysql_fetch_row(res_ptr);
		if (result_row[5]==0)
		{
			return 0;//没有库存
		}
		else
		{
			query = "update book set stock=stock-1 where book_id = '" + book_id + "';";
			y = mysql_query(&sql, query.c_str());
			if (y == 1) return 1;	//没有更新成功
			time_t tt = time(NULL);
			struct tm* t = localtime(&tt);
			string borrow_date = to_string(t->tm_year + 1900) + '.' + to_string(t->tm_mon + 1) + '.' + to_string(t->tm_mday);
			string return_date;
			if (t->tm_mon == 11) {
				string return_date = to_string(t->tm_year + 1900+1) + '.' + to_string(1) + '.' + to_string(t->tm_mday);
			}
			else {
				string return_date = to_string(t->tm_year + 1900) + '.' + to_string(t->tm_mon + 2) + '.' + to_string(t->tm_mday);
			}
			query = "insert into borrow values('" + user_id + "','" + book_id + "','" + 
				borrow_date.substr(0,10) + "','" + return_date.substr(0,10) + "');";
			y = mysql_query(&sql, query.c_str());
			if (y == 1) {
				query = "update book set stock=stock+1 where book_id = '" + book_id + "';";
				mysql_query(&sql, query.c_str());
				return 2;	//没有更新书的stock成功
			}
			else return 3;	//借阅成功
		}
	}
	int returnbook(string & user_id,string & book_id, MYSQL &sql) {  //user manager

		string query;

		query = "delete from borrow where user_id='" + user_id + "'and book_id = '" + book_id + "';";
		int y = mysql_query(&sql, query.c_str());
		if (y == 0)
		{
			query = "update book set stock=stock+1 where book_id = '" + book_id + "';";
			y = mysql_query(&sql, query.c_str());
			return y == 0;
		}
		else return y == 0;

	}
	bool deleteuser(User & _user, MYSQL &sql) {        //manager
		string query;
		query = "delete from user where id = '" + _user.id + "';";
		int r = mysql_query(&sql, query.c_str());
		return r == 0;
	}
	bool deletebook(string& book_id, MYSQL& sql) {            //manager
		MYSQL_RES *res_ptr;
		MYSQL_ROW result_row;
		int  y;
		string query = "select * from borrow where book_id ='" + book_id + "';";
		y = mysql_query(&sql, query.c_str());
		res_ptr = mysql_store_result(&sql);
		result_row = mysql_fetch_row(res_ptr);
		//row = mysql_num_rows(res_ptr);
		if (result_row == NULL) {
			query = "delete from book where book_id = '" + book_id + "';";
			int x = mysql_query(&sql, query.c_str());
			return x == 0;
		}
		else return false;
	}

	bool SelectBorrow(Borrow& q, MYSQL& sql) {
		string query;
		MYSQL_RES *res_ptr;
		MYSQL_ROW result_row;
		int row;
		query = "select borrow_date,return_date from borrow where user_id='" + q.user_id + "'and book_id='" + q.book_id + "';";
		mysql_query(&sql, query.c_str());
		res_ptr = mysql_store_result(&sql);
		row = mysql_num_rows(res_ptr);
		if (row == NULL) return 0;
		else {
			result_row = mysql_fetch_row(res_ptr);
			q.borrow_date = result_row[0];
			q.return_date = result_row[1];
			return 1;
		}
	}

	
};

#endif // !_manager_H_
