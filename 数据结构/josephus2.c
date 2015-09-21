/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com  or  jungzhang@xiyoulinux.org
 * Last modified : 2015-09-13 19:07
 * Filename	 : josephus2.c
 * Description	 : n固定m由结点自带
 * *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct josephus{
	
	int num;
	int m;
	struct josephus *pNext;
	
}JOSE;

//初始化链表
JOSE *InitList(void)
{
	JOSE *pHead;

	pHead = (JOSE *)malloc(sizeof(JOSE));
	pHead->pNext = pHead;
	
	return pHead;
}

//创建一条约瑟夫环(将尾节点与首节点相连的局部循环链表)
int CreatList(JOSE *pHead, int n)
{
	int i;
	JOSE *pNew,*pTail;

	pTail = pHead;
	for (i = 1; i <= n; i++){
		if ((pNew = (JOSE *)malloc(sizeof(JOSE))) == NULL)
			return 0;
		pNew->num = i;
		while(1){
			printf("请输入第%d个结点带的m值：",i);
			scanf("%d", &pNew->m);
			if (pNew->m > 0){
				break;
			}
			else{
				printf("m值输入有误!\n");
			}
		}
		pTail->pNext = pNew;
		pTail = pNew;
	}
	pTail->pNext = pHead->pNext;
	
	return 1;
}

//按要求弹出结点
void PopList(JOSE *pHead, int n, int m)
{
	int i;
	JOSE *pTar,*pTemp = pHead;

	while(n){
		for (i = 0; i < m - 1; i++){
			pTemp = pTemp->pNext;
		}
		pTar = pTemp->pNext;
		pTemp->pNext = pTar->pNext;
		printf("%d\t", pTar->num);
		m = pTar->m;
		free(pTar);
		n--;
	}
	free(pHead);
}

int main(void)
{
	JOSE *pHead;
	int m, n;
	pHead = InitList();
	printf("请输入n = ");
	scanf("%d", &n);
	if (n > 0){
		if (!CreatList(pHead, n))
			return -1;
		while(1){
			printf("请输入初始m = ");
			scanf("%d", &m);
			if (m > 0){
				break;
			}
			else{
				printf("m值输入有误!\n");
			}
		}
		PopList(pHead, n, m);
		printf("\n");
	}
	
	return 0;
}
