#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//引用登陆用户的全局数据结构
extern account_t gl_CurUser;

static const int PLAY_PAGE_SIZE = 5;

//根据计划ID，显示演出票
void Sale_UI_ShowTicket(int schID){
	char choice;int i;
	ticket_list_t list;
	ticket_list_t pTemp;
	char status[10];
	play_t play;
	schedule_t sch;
	studio_t stu;
	seat_list_t seat_l;
	seat_t seat;
	Pagination_t paging;

	List_Init(seat_l,seat_node_t);
	List_Init(list,ticket_node_t);
	
	Schedule_Srv_FetchByID(schID, &sch);
	Play_Srv_FetchByID(sch.play_id, &play);
	Studio_Srv_FetchByID(sch.studio_id, &stu);
	Seat_Srv_FetchByRoomID(seat_l,sch.studio_id);
	paging.offset = 0;
	paging.pageSize = 10;
	paging.totalRecords = Ticket_Srv_FetchBySchID(list,schID);
	Paging_Locate_FirstPage(list, paging);
	do{
		system("clear");
	printf("|=============================================================================================================================|\n");
	printf("|-------------------------------------------------------票 信 息 输 出--------------------------------------------------------|\n");
	printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
                printf("%10s%20s%20s%20s%20s%20s%20s%20s\n","票ID","演出计划ID","剧目名称","演出厅名称","座位ID","座位行列号","票价","座位状态");
	printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
	Paging_ViewPage_ForEach(list, paging, ticket_node_t, pTemp, i)
	{
		Seat_Srv_FetchByID(pTemp->data.seat_id,&seat);
		if (pTemp->data.status == (ticket_status_t)0)
      			strcpy(status,"待售");
		else if (pTemp->data.status == (ticket_status_t)1)
       			strcpy(status,"已售");
		else if (pTemp->data.status == (ticket_status_t)9)
              		strcpy(status,"预留");
		printf("%9d%12d%18s%15s%18d          (%d,%d) %18d%16s\n",pTemp->data.id,pTemp->data.schedule_id,play.name,stu.name,pTemp->data.seat_id,seat.row,seat.column,pTemp->data.price,status);
	}
		printf("|------------------------ 总条数:%2d ------------------------------------------------------ 页码 %2d/%2d ------------------------|\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("|-----------------------------[P]上一页   |  [N]下一页   |   [r]返回上一级　｜　[b]售票---------------------------------------|\n");
		printf("|=============================================================================================================================|\n");
		printf("请选择:");
	        scanf("%c",&choice);	flu();
		switch(choice)
		{
			case 'b':
			case 'B':
				if(Sale_UI_SellTicket(list,seat_l))
				{
					printf("正在重载票....\n");
					if (Ticket_Srv_FetchBySchID(list,schID))
						printf("票信息重载成功!\n");
				}
				printf("输入Enter返回...");	flu();
				break;
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging)) {
					Paging_Locate_OffsetPage(list, paging, -1, ticket_node_t);
				}
				break;
			case 'N':
			case 'n':
				if (!Pageing_IsLastPage(paging)) {
					Paging_Locate_OffsetPage(list, paging, 1, ticket_node_t);
				}
				break;
			//case 'r':
			//case 'R':
			//	return;
		}
	}while(choice != 'R' && choice != 'r');
}

inline int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList){
	int row,column;
	char key;
	ticket_t buf;
	sale_t buf_s;	
	ticket_list_t pTemp = tickList->next;
	seat_list_t temp = seatList->next;
	printf("请输入座位的行号:");
	scanf("%d",&row);	flu();
	printf("请输入座位的列号:");
	scanf("%d",&column);	flu();
	while(temp != seatList)
	{
		if ((temp->data.row == row) && (temp->data.column == column))
		{	
			while(pTemp != tickList)
			{
				if (temp->data.id == pTemp->data.seat_id)
				{
					if((ticket_status_t)1 != pTemp->data.status)
					{
						printf("是否确定售出票(n放弃)？");
						scanf("%c",&key); flu();
						if(key!='n' && key != 'N')
						{
							pTemp->data.status = (ticket_status_t)1;
							if (Ticket_Srv_Modify(&(pTemp->data)))
							{	
							//	if (Sale_Srv_Add(&(pTemp->data))) 	
									printf("成功售出!\n");
								return 1;
							}
						}
						else{printf("用户放弃!\n");	return 0;}
					}
					else
					{printf("该票已卖出!\n");return 0;}
				}
				
				pTemp = pTemp->next;
			}
			printf("未找到该座位对应的票!\n");	return 0;
		}
		temp = temp->next;
	}
	printf("未找到该座位!\n");	return 0;
			
}



//根据剧目ID显示演出计划
void Sale_UI_ShowScheduler(int playID)
{
	
	play_t playbuf;		
	int i,tempid;	char choice;
	play_list_t playlist;
	schedule_list_t schelist;
	schedule_node_t *pos;
	schedule_t buf;
	play_t play;
	studio_t studio;

	Play_Srv_FetchByID(playID,&playbuf);	//存储playID对应的剧目信息到playbuf中
	List_Init(playlist,play_node_t);	//初始化剧目链表
	List_Init(schelist,schedule_node_t);
	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = 5;
	paging.totalRecords = Schedule_Srv_FetchByPlay(schelist,playID);
	Paging_Locate_FirstPage(schelist, paging);
	
	do{system("clear");
		printf("|=============================================================================================================================|\n");
		printf("|----------------------------------------------------演 出 计 划 信 息--------------------------------------------------------|\n");
		printf("%10s%15s%23s%20s%22s%18s%23s\n","ID","剧目ID","剧目名字","演出厅ID","演出厅名称","演出日期","演出时间");
		Paging_ViewPage_ForEach(schelist, paging, schedule_node_t, pos, i){
			if (Play_Srv_FetchByID(pos->data.play_id,&play) == 0)
			{
				printf("获取剧目失败!\n");
			}
			if (Studio_Srv_FetchByID(pos->data.studio_id,&studio) == 0)
			{
				printf("获取演出厅失败!\n");
			}
			printf("%10d%10d%20s%15d%18s%10d年%2d月%2d日%8d时%2d分%2d秒\n",pos->data.id,pos->data.play_id,play.name,pos->data.studio_id,studio.name,pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute,pos->data.time.second);
		}
		printf("------------------------总条数:%2d -------------------------------------------------------- 页数 %2d/%2d -------------------------\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|-----------------------[T]票务信息   |     [P]上一页    |     [N]下一页     |     [R]返回上一级------------------------------|\n");
		printf("|=============================================================================================================================|\n");
		printf("请选择:");
	scanf("%c",&choice);
	flu();
	switch(choice)
	{
		case 't':
		case 'T':
			printf("请输入演出计划ID:");	scanf("%d",&tempid);	flu();
			if (Schedule_Srv_FetchByID(tempid,&buf))
				Sale_UI_ShowTicket(tempid);
			else{
				printf("该ID所对应的演出计划不存在!\n输入Enter键返回:");	flu();
			}
			break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging))
			{
				Paging_Locate_OffsetPage(schelist, paging, -1, schedule_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging))
			{
				Paging_Locate_OffsetPage(schelist, paging, 1, schedule_node_t);
			}
			break;
	}
	}while (choice != 'r' && choice != 'R');
}


void Sale_UI_MgtEntry(void) {

	system("clear");
	int play_id,i;
	char play_name[31];
	char filter_name[31];
	char type[10];
	char rating[10];
        play_list_t list,pos1,pName1,pName;
	List_Init(list,play_node_t);
	List_Init(pName1,play_node_t);

	Pagination_t paging;
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;
	paging.totalRecords = Play_Srv_FetchAll(list);
	Paging_Locate_FirstPage(list,paging);
	flu();

        char choice;
	do{
		system("clear");
		printf("\n|=============================================================================================================================|\n");
		printf("|*******************************************************剧 目 信 息***********************************************************|\n");
		printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|%6s%25s%20s%15s%15s%15s%20s%20s%15s|\n","剧目ID","剧目名称","剧目类型","地区","适宜人群","时长","上映时间","下线日期","票价");
		Paging_ViewPage_ForEach(list,(paging),play_node_t,pos1,i)
		{
			if (pos1->data.type == (play_type_t)1)
				strcpy(type,"电影");
			else if (pos1->data.type == (play_type_t)2)
				strcpy(type,"戏剧");
			else if (pos1->data.type == (play_type_t)3)
				strcpy(type,"音乐");
			if (pos1->data.rating == (play_rating_t)1)
        		        strcpy(rating,"儿童");
        		else if (pos1->data.rating == (play_rating_t)2)
        		        strcpy(rating,"青年");
        		else if (pos1->data.rating == (play_rating_t)3)
        		        strcpy(rating,"成人");
			printf("%5d%20s%20s%15s%10s%14d分%8d年%2d月%2d日%8d年%2d月%2d日%7d\n",pos1->data.id,pos1->data.name,type,pos1->data.area,rating,pos1->data.duration,pos1->data.start_date.year,pos1->data.start_date.month,pos1->data.start_date.day,pos1->data.end_date.year,pos1->data.end_date.month,pos1->data.end_date.day,pos1->data.price);
		}
		printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|---------------总条数:%2d ------------------------------------------------------------------------------------ 页数 %2d/%2d ----|\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
	printf("|----------[C|c]显示演出计划 | [S|s]查询剧目名字 | [F|f]过滤名字输入 | [P|p]前一页 | [N|n]下一页 | [R|r]返回上一页 -----------|\n");
	printf("\n|=============================================================================================================================|\n");	
		printf("请选择：");
        	scanf("%c",&choice);flu();
       		 switch(choice)
        	{
                	case 'c':
                	case 'C':
				printf ("请输入剧目ID：");
				scanf ("%d",&play_id);	flu();
				Sale_UI_ShowScheduler(play_id);
				break;
			case 's':
			case 'S':
				printf("剧目名称：");
				scanf("%s",play_name);	flu();
				Play_Srv_FetchByName(pName1,play_name);
				printf("\n|=============================================================================================================================|\n");
		printf("|*******************************************************剧 目 信 息***********************************************************|\n");
		printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|%6s%25s%20s%15s%15s%15s%20s%20s%15s|\n","剧目ID","剧目名称","剧目类型","地区","适宜人群","时长","上映时间","下线日期","票价");
				pName = pName1->next;
				while(pName != pName1){
				if (pName->data.type == (play_type_t)1)
					strcpy(type,"电影");
				else if (pName->data.type == (play_type_t)2)
				strcpy(type,"戏剧");
				else if (pName->data.type == (play_type_t)3)
					strcpy(type,"音乐");
				if (pName->data.rating == (play_rating_t)1)
        			        strcpy(rating,"儿童");
        			else if (pName->data.rating == (play_rating_t)2)
        			        strcpy(rating,"青年");
        			else if (pName->data.rating == (play_rating_t)3)
        			        strcpy(rating,"成人");
				printf("%5d%20s%20s%15s%10s%14d分%8d年%2d月%2d日%8d年%2d月%2d日%7d\n",pName->data.id,pName->data.name,type,pName->data.area,rating,pName->data.duration,pName->data.start_date.year,pName->data.start_date.month,pName->data.start_date.day,pName->data.end_date.year,pName->data.end_date.month,pName->data.end_date.day,pName->data.price);
				pName = pName->next;
				printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
				}
				printf("按Enter键继续......");	flu();
				break;
			case 'f':
			case 'F':
				printf("需要过滤的剧目名称：");
				scanf("%s",filter_name);	flu();
				Play_Srv_FilterByName(pName1,filter_name);
				printf("\n|=============================================================================================================================|\n");
		printf("|*******************************************************剧 目 信 息***********************************************************|\n");
		printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
		printf("|%6s%25s%20s%15s%15s%15s%20s%20s%15s|\n","剧目ID","剧目名称","剧目类型","地区","适宜人群","时长","上映时间","下线日期","票价");
				pName = pName1->next;
				while(pName != pName1){
				if (pName->data.type == (play_type_t)1)
					strcpy(type,"电影");
				else if (pName->data.type == (play_type_t)2)
				strcpy(type,"戏剧");
				else if (pName->data.type == (play_type_t)3)
					strcpy(type,"音乐");
				if (pName->data.rating == (play_rating_t)1)
        			        strcpy(rating,"儿童");
        			else if (pName->data.rating == (play_rating_t)2)
        			        strcpy(rating,"青年");
        			else if (pName->data.rating == (play_rating_t)3)
        			        strcpy(rating,"成人");
				printf("%5d%20s%20s%15s%10s%14d分%8d年%2d月%2d日%8d年%2d月%2d日%7d\n",pName->data.id,pName->data.name,type,pName->data.area,rating,pName->data.duration,pName->data.start_date.year,pName->data.start_date.month,pName->data.start_date.day,pName->data.end_date.year,pName->data.end_date.month,pName->data.end_date.day,pName->data.price);
				pName = pName->next;
				printf("|-----------------------------------------------------------------------------------------------------------------------------|\n");
				}
				printf("按Enter键继续......");	flu();
				break;
			case 'p':
			case 'P':
				if(!Pageing_IsFirstPage(paging)){
				Paging_Locate_OffsetPage(list,paging,-1,play_node_t);
			}
				break;
			case 'n':
			case 'N':
				if(!Pageing_IsLastPage(paging)){
				Paging_Locate_OffsetPage(list,paging,1,play_node_t);
			}
					break;
			case 'r':
			case 'R':
				return;
	        }
	}while(choice != 'r' || choice != 'R');
}

//退票
void Sale_UI_ReturnTicket()
{
	int ID;
	ticket_t rec;
	sale_t temp;
	printf("请输入票的ID:");
	scanf("%d",&ID);	flu();
	if(!Ticket_Srv_FetchByID(ID,&rec))
	{
		printf("此ID不存在，无法退票!\n");
		printf("按Enter键返回.....");	flu();
		return;
	}
	else
	{
		if(rec.status!=(ticket_status_t)1)
		{
			printf("该票未售出，无法退票!\n");
			printf("按Enter键返回.....");	flu();
			return;
		}
		else
		{
			rec.status = (ticket_status_t)0;
			if(!Ticket_Srv_Modify(&rec))
			{
				printf("退票失败.\n");
			}
			else
			{
				printf("退票成功.\n");
				Sale_Srv_Add(&temp);
			}
			printf("按Enter键返回.....");	flu();
		}
	}
	
}
