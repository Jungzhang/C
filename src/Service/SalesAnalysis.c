/*
 * salesanalysis.c
 *
 *  Created on: 2015年6月12日
 *      Author: Administrator
 */
#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
#include "../Persistence/Sale_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"

#include "../Common/List.h"

//计算员工usrID在给定时间区间的销售额
inline int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate){
	// 请补充完整
       return 1;
}

//统计销售数据
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) {

	// 请补充完整
       return 1;
}

//根据票房排序
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list){
	// 请补充完整
}

/*

//实现销售数据分析，获取分析好的销售数据，写入销售分析（salesanalysis_list_t slist）链表中
int SalesAnalysis_Srv_Analysis(play_list_t plist, sale_list_t salist,
		salesanalysis_list_t slist) {
	// 请补充完整
       return 1;
}

//销售分析链表（slist）结点数据，按剧目票销售数量（sales）字段降序排序
//sanalysisrec ：销售分析链表（slist）结点数量
void SalesAnalysis_Srv_Sort(salesanalysis_list_t slist, int sanalysisrec) {
	// 请补充完整
}

//根据剧目（play）的id号找到Sale.dat中匹配的记录,返回记录总数，即售票数
long SalesAnalysis_Srv_CountTicketByID(sale_list_t list, int playID) {
	// 请补充完整
       return 1;
}

inline int Salesanalysis_Srv_Insert(const salesanalysis_t *data) {
	// 请补充完整
       return 1;
}
inline int SalesAnalysis_Srv_FetchAll(salesanalysis_list_t list) {
	// 请补充完整
       return 1;
}
inline int Sale_Srv_FetchAll(sale_list_t list) {
	// 请补充完整
       return 1;
}
*/
