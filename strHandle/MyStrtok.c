/*************************************************
***�������� unPackLineBuff
***���ߣ�   VANGE
***���������
***param1: �ļ�������
***param2: �ļ��ָ��ַ�
***param3: ���ַ��滻�ַ�
***param4: �ı���������
***
***�����0-�ɹ� -1-ʧ�� 
***����˵���� ���ļ��ڵ��ַ���������ָ����ָ������
************************************************/
int unPackLineBuff(char *pLineBuff, char *SplitC, char ReplaceC, int ColNum)
{
	char *beg=pLineBuff;
	char *end=pLineBuff;
	int  iCnt = 0, i = 0, offset = 0, LenSplitC = 0;
	char sTxt[ColNum][64];    // ��ά���ݵ�������sTxt[64]; [ColNum] ��ʾ��sTxt[64]������
	char (*pTxt)[64] = sTxt;  // ��ά����ָ��
	
	LenSplitC = strlen(SplitC);
	memset(sTxt, 0x00 ,sizeof(sTxt));
	while(1)
	{
		/* 1. end == �ַ����ָ�� */
		if(0 == memcmp(end, SplitC, LenSplitC))
		{
			iCnt = end - beg;
			offset = beg - pLineBuff + LenSplitC;
			/* 1.1 beg == �ָ�� */
			if(0 == memcmp(beg, SplitC, LenSplitC))
			{
				/* ���ڵĵ�1��Ϊ�� */
				if(0 == iCnt)
				{
					memcpy(*(pTxt+i)+0, &ReplaceC, 1);	
				}
				/* ���ڵĵ�n��Ϊ�� n > 1 */
				if(LenSplitC == iCnt)
				{
					memcpy(*(pTxt+i)+0, &ReplaceC, 1);	
				}
				/* ���ڵĵ�n��Ϊ�� , n > 1*/
				if(iCnt > LenSplitC)
				{
					memcpy(*(pTxt+i), pLineBuff + offset, iCnt-LenSplitC);
				}
			}
			/* 1.2 beg != �ָ�� */
			else
			{
				/* ���ڵ����ݣ���һ�в�Ϊ�� *(pTxt+i) = pTxt[i] */
				memcpy(*(pTxt+i), pLineBuff, iCnt);	
			}
			beg = end; i++;
			end = end + LenSplitC;
		}
		/* 2. end=='\n' �����ַ��Ѿ�������� */
		else if(*end == '\n')
		{
			int ColType = 1;
			iCnt = end - beg;
			offset = beg - pLineBuff + LenSplitC;
			/* 2.1 ����ָ��Ϊ'.', ��ʽ1�� "....aaa.\n" , '.'��'\n' ֮�仹��һ������ֵ  */
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
			/* 2.2 ����ָ��Ϊ'.', ��ʽ2�� "....aaa.\n" , '.'��'\n' ֮��û����ֵ��  */
			else
			{
				break;	
			}
			break;	
		}
		/* 3. end ָ���������� */
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
***�������� GetFileInfo
***���ߣ�   VANGE
***���������
***param1: �ļ���(ȫ·��)
***param2: �ļ��ָ��ַ�
***param3: ���ַ��滻�ַ�
***param4: ָ����ָ������
***
***�����0-�ɹ� -1-ʧ�� 
***����˵���� ���ļ��ڵ��ַ���������ָ����ָ������
************************************************/
int GetFileInfo(char *pPathName, char *SplitC, char ReplaceC, int ColNum)
{
	
	char sLineBuf[1024 +1];
	struct stat s;
	int iRet, i=0;
	
	/* 1.���ļ� */
	FILE * fp = fopen(pPathName, "r");
    if(NULL == fp)
    {
    	printf("[%s][%d]���ļ�[%s]ʧ��",__FILE__,__LINE__,pPathName);
    	return -1;s
    }
    /* 2.��ȡ�ļ����� */                                                                                                 
    if (stat(pPathName,&s)==-1) /* fstat(fp, &s) */                                    
    {                                                                      
        printf("��ȡ�ļ�[%s]����ʧ��",pPathName);
        fclose(fp);
        return -1;                    
    }
    /* 3.�ļ�����,���Ҳ�Ϊ�� */                                          
	if (access(openFile.c_str(),F_OK)==0 && s.st_size!=0)
	{
		while(NULL!=fgets(sLineBuf, sizeof(sLineBuf)-1, fp))
		{
			/* ���н����ļ����� */
			printf("[%s][%d]�����ļ�[%s]��[%d]��===beg===\n",__FILE__,__LINE__,pPathName,i+1);
			if(unPackLineBuff(sLineBuf,SplitC,ReplaceC,ColNum));
			{
				printf("[%s][%d]�����ļ�[%s]��[%d]��ʧ��",__FILE__,__LINE__,pPathName,i+1);
				fclose(fp);
				return -1	
			}
			printf("[%s][%d]�����ļ�[%s]��[%d]��===end===\n",__FILE__,__LINE__,pPathName,i+1);
		}
		if(ferror(fp))
		{
			printf("[%s][%d]�����ļ�[%s]��[%d]��===end===\n",__FILE__,__LINE__,pPathName,i+1);
			fclose(fp);
			return -1
		}
	}
	
	return 0;	
}

int main(int argc, char **argv)
{
	char *pStr[30];  //ָ������,���ÿ�е�����
	if( argc != 3)
		printf("./a.out param1 ��β���\n");
	
	int ColNum = atoi(argv[2]);
	if(GetFileInfo(argv[1], ".",'&',ColNum) < 0)
	{
		printf("[%s][%d]�������ó���",__FILE__,__LINE__);
		return -1;
	}
	return 0;
}

/*
Ϊ�˸��õ����ָ��Ͷ�ά����Ĺ�ϵ��������������һ��ָ�� a ��ָ����� p��

int (*p)[4] = a;
�����е�*���� p ��һ��ָ�룬��ָ��һ�����飬���������Ϊint [4]�������� a ��������ÿ��һά��������͡�

    #include <stdio.h>
    int main(){
        int a[3][4] = { {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11} };
        int (*p)[4] = a;
        printf("%d\n", sizeof(*(p+1)));
        return 0;
    }




*/