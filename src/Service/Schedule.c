#include "Schedule.h"
#include "../Common/List.h"
#include "../Persistence/Schedule_Persist.h"
#include "Ticket.h"
#include <stdio.h>
//添加数据为data的新演出计划服务
inline int Schedule_Srv_Add(const schedule_t *data) {
	int rtn = 0;
	rtn = Schedule_Perst_Insert(data);	
	if (Ticket_Srv_AddBatch(data->id,data->studio_id)){
		
		return rtn;
	}
	else
		return 0;
}
//修改新数据为data的演出计划服务
inline int Schedule_Srv_Modify(const schedule_t *data) {
	int rtn = 0;
	rtn = Schedule_Perst_Update(data);

	return rtn;
}
//删除主键为id的演出计划服务
inline int Schedule_Srv_DeleteByID(int ID) {
	int rtn = 0;
	rtn = Schedule_Perst_DeleteByID(ID);
	
	if (Ticket_Srv_DeleteBatch(ID) == 0)
	{
		printf("删除票信息失败!\n");	return 0;
	}
	
	return rtn;
}
//按照ＩＤ从文件中查询演出计划数据，并将其数据保存在buf所指的内存单元中
inline int Schedule_Srv_FetchByID(int ID, schedule_t *buf) {
	return(Schedule_Perst_SelectByID(ID,buf));

}
//从文件中获取所有演出计划数据组织成头指针为list的链表
inline int Schedule_Srv_FetchAll(schedule_list_t list) {
	return (Schedule_Perst_SelectAll(list));
}
//从文件中获取与主键为play_id相关的所有演出计划数据组织成头指针为list的链表
inline int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id) {
	return (Schedule_Perst_SelectByPlay(list,play_id));
}

//根据演出ID，统计上座率及票房，返回票房数收入
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount, int *totalCount) {
	// 请补充完整
       return 1;
}

