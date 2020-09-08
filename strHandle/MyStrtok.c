/*************************************************
***函数名： unPackLineBuff
***作者：   VANGE
***输入参数：
***param1: 文件行内容
***param2: 文件分隔字符
***param3: 空字符替换字符
***param4: 文本单行列数
***
***输出：0-成功 -1-失败 
***功能说明： 将文件内的字符，解析到指定的指针数组
************************************************/
int unPackLineBuff(char *pLineBuff, char *SplitC, char ReplaceC, int ColNum)
{
	char *beg=pLineBuff;
	char *end=pLineBuff;
	int  iCnt = 0, i = 0, offset = 0, LenSplitC = 0;
	char sTxt[ColNum][64];    // 二维数据的类型是sTxt[64]; [ColNum] 表示是sTxt[64]的数组
	char (*pTxt)[64] = sTxt;  // 二维数组指针
	
	LenSplitC = strlen(SplitC);
	memset(sTxt, 0x00 ,sizeof(sTxt));
	while(1)
	{
		/* 1. end == 字符串分割符 */
		if(0 == memcmp(end, SplitC, LenSplitC))
		{
			iCnt = end - beg;
			offset = beg - pLineBuff + LenSplitC;
			/* 1.1 beg == 分割符 */
			if(0 == memcmp(beg, SplitC, LenSplitC))
			{
				/* 行内的第1列为空 */
				if(0 == iCnt)
				{
					memcpy(*(pTxt+i)+0, &ReplaceC, 1);	
				}
				/* 行内的第n列为空 n > 1 */
				if(LenSplitC == iCnt)
				{
					memcpy(*(pTxt+i)+0, &ReplaceC, 1);	
				}
				/* 行内的第n列为空 , n > 1*/
				if(iCnt > LenSplitC)
				{
					memcpy(*(pTxt+i), pLineBuff + offset, iCnt-LenSplitC);
				}
			}
			/* 1.2 beg != 分割符 */
			else
			{
				/* 行内的数据，第一列不为空 *(pTxt+i) = pTxt[i] */
				memcpy(*(pTxt+i), pLineBuff, iCnt);	
			}
			beg = end; i++;
			end = end + LenSplitC;
		}
		/* 2. end=='\n' 单行字符已经在最后了 */
		else if(*end == '\n')
		{
			int ColType = 1;
			iCnt = end - beg;
			offset = beg - pLineBuff + LenSplitC;
			/* 2.1 假设分格符为'.', 格式1： "....aaa.\n" , '.'和'\n' 之间还有一个空列值  */
			if(1 == ColType)
			{
				if(1 == iCnt)
				{
					memcpy(*(pTxt+i)+0, &ReplaceC, 1);
				}
				if(iCnt > 1)
				{
					memcpy(*(pTxt+i), pLineBuff + offset, iCnt-LenSplitC);
				}
				
			}
			/* 2.2 假设分格符为'.', 格式2： "....aaa.\n" , '.'和'\n' 之间没有列值了  */
			else
			{
				break;	
			}
			break;	
		}
		/* 3. end 指向行内数据 */
		else
		{
			end++;	
		}
		
	}
	for(i = 0; i < ColNum; i++)
	{
		printf("[%s][%d] pTxt[%d]=%s\n",__FILE__,__LINE__,i,*(pTxt+i));	
	}		
	return 0;	
}

/*************************************************
***函数名： GetFileInfo
***作者：   VANGE
***输入参数：
***param1: 文件名(全路径)
***param2: 文件分隔字符
***param3: 空字符替换字符
***param4: 指定的指针数组
***
***输出：0-成功 -1-失败 
***功能说明： 将文件内的字符，解析到指定的指针数组
************************************************/
int GetFileInfo(char *pPathName, char *SplitC, char ReplaceC, int ColNum)
{
	
	char sLineBuf[1024 +1];
	struct stat s;
	int iRet, i=0;
	
	/* 1.打开文件 */
	FILE * fp = fopen(pPathName, "r");
    if(NULL == fp)
    {
    	printf("[%s][%d]打开文件[%s]失败",__FILE__,__LINE__,pPathName);
    	return -1;s
    }
    /* 2.获取文件属性 */                                                                                                 
    if (stat(pPathName,&s)==-1) /* fstat(fp, &s) */                                    
    {                                                                      
        printf("获取文件[%s]属性失败",pPathName);
        fclose(fp);
        return -1;                    
    }
    /* 3.文件存在,并且不为空 */                                          
	if (access(openFile.c_str(),F_OK)==0 && s.st_size!=0)
	{
		while(NULL!=fgets(sLineBuf, sizeof(sLineBuf)-1, fp))
		{
			/* 逐行解析文件内容 */
			printf("[%s][%d]解析文件[%s]第[%d]行===beg===\n",__FILE__,__LINE__,pPathName,i+1);
			if(unPackLineBuff(sLineBuf,SplitC,ReplaceC,ColNum));
			{
				printf("[%s][%d]解析文件[%s]第[%d]行失败",__FILE__,__LINE__,pPathName,i+1);
				fclose(fp);
				return -1	
			}
			printf("[%s][%d]解析文件[%s]第[%d]行===end===\n",__FILE__,__LINE__,pPathName,i+1);
		}
		if(ferror(fp))
		{
			printf("[%s][%d]解析文件[%s]第[%d]行===end===\n",__FILE__,__LINE__,pPathName,i+1);
			fclose(fp);
			return -1
		}
	}
	
	return 0;	
}

int main(int argc, char **argv)
{
	char *pStr[30];  //指针数组,存放每行的数据
	if( argc != 3)
		printf("./a.out param1 入参不对\n");
	
	int ColNum = atoi(argv[2]);
	if(GetFileInfo(argv[1], ".",'&',ColNum) < 0)
	{
		printf("[%s][%d]函数调用出错",__FILE__,__LINE__);
		return -1;
	}
	return 0;
}

/*
为了更好的理解指针和二维数组的关系，我们先来定义一个指向 a 的指针变量 p：

int (*p)[4] = a;
括号中的*表明 p 是一个指针，它指向一个数组，数组的类型为int [4]，这正是 a 所包含的每个一维数组的类型。

    #include <stdio.h>
    int main(){
        int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
        int (*p)[4] = a;
        printf("%d\n", sizeof(*(p+1)));
        return 0;
    }




*/