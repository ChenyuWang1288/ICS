#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<bitset>
#include<iostream>
#include<set>
using namespace std;
set<string> instructionSet;
/*struct label*/
struct label{
    char name[20];
    int location;
    struct label *next;
};
/*struct linecontent to save the input content*/
struct linecontent{
    char linechar[100];
    int no;
    struct linecontent *next;
};
struct label *head, *tail;
struct label *p1, *p2;
struct linecontent *firstline;
struct linecontent *pr1, *pr2;
struct linecontent *pr;
char end1[20] = ".END";
char line[100];/*record the line that are scaned currently*/
int currentline;
int number = 0; /*record the number of labels*/
int linenumber = -1; /*record the number of lines*/
int addnumber = 1;
void InitializeSet();
void CreateSymbolTable();
void Assembler();
void ScanLine();
void CreateNewLabel();
void SaveLine();
void Translate();
void PrintfRegister(char reg);
void HextoBin(char *vect, int bitnumber);
int DecimaltoBin(char *decimal);
int Hex(char *hex);
void PrintBitset(char a);
void A(char *a, int i);
void N(char *a, int i);
void L(char *a, int i);
void S(char *a, int i);
void T(char *a, int i);
void B(char *a, int i);
void J(char *a, int i);
void R(char *a, int i);
void H(char *a, int i);
void Point(char *a, int i);
void orig(char *a);
void Fill(char *a);
void blkw(char *a);
void stringz(char *a);
void StringPrint(char *str);
void G(char *a);
void O(char *a);
void P(char *a);
void I(char *a);
/*main program*/
int main()
{
    InitializeSet();
    head = tail = NULL;
    CreateSymbolTable();
    pr = firstline;
    p1 = head;

    /* print lines saved in link */
    //     while(pr != NULL)
    //    {
    //         puts(pr->linechar);
    //        printf("%d\n", pr->no);
    //        pr = pr->next;
    //   }
    //  while(p1 != NULL)
    //   {
    //       puts(p1->name);
    //       printf("%d\n", p1->location);
    //       p1 = p1->next;
    // }

    Assembler();
    
    return 0;
}

void InitializeSet()
{
    instructionSet.insert("ADD");
    instructionSet.insert("AND");
    instructionSet.insert("NOT");
    instructionSet.insert("LD");
    instructionSet.insert("LDR");
    instructionSet.insert("LDI");
    instructionSet.insert("LEA");
    instructionSet.insert("ST");
    instructionSet.insert("STR");
    instructionSet.insert("STI");
    instructionSet.insert("TRAP");
    instructionSet.insert("trap");
    instructionSet.insert("RET");
    instructionSet.insert("JSRR");
    instructionSet.insert("BR");
    instructionSet.insert("BRn");
    instructionSet.insert("BRz");
    instructionSet.insert("BRp");
    instructionSet.insert("BRnz");
    instructionSet.insert("BRzp");
    instructionSet.insert("BRnp");
    instructionSet.insert("BRnzp");
    instructionSet.insert("JMP");
    instructionSet.insert("JSR");
    instructionSet.insert("RTI");
    instructionSet.insert(".ORIG");
    instructionSet.insert(".FILL");
    instructionSet.insert(".BLKW");
    instructionSet.insert(".STRINGZ");
    instructionSet.insert(".END");
    instructionSet.insert("GETC");
    instructionSet.insert("OUT");
    instructionSet.insert("PUTS");
    instructionSet.insert("IN");
    instructionSet.insert("PUTSP");
    instructionSet.insert("HALT");
}

void CreateSymbolTable()
{
    ScanLine();
}

void Assembler()
{
    pr = firstline;
    /*traverse and translate*/
    while(1)
    {
        currentline = pr->no;
        Translate();
        pr = pr->next;
        if(pr == NULL)
        break;
    }
}

/*gets a line into line[50]*/
void ScanLine()
{
    int i;
    char ch[20];
    char line1[100];
    /*scan the code*/
    for(i = 0; ; i++)
    {
        int j = 0;
        linenumber += addnumber;
        /* empty the line */
        for(j = 0; j < 100; j++)
        {
            line1[j] = ' ';
            line[j] = ' ';
        }

        fgets(line1, 50, stdin);
        j = 0;
        while(line1[j] != '\n') j++;
        line1[j] = '\0';

        int k = 0, m = 0;
        j = 0;
        /*delet empty spaces*/
        if(line1[0] == '\0') /* if it is an empty line */
        {
            linenumber = linenumber - addnumber;
            continue;
        }
        while(line1[k] ==' ') k++;
        if(line1[k] == '\0') /* if it is an empty line */
        {
            linenumber = linenumber - addnumber;
            continue;
        }
        for(m = k; line1[m] != '\0'; m++)
        {
            line[j] = line1[m];
            j++;
        }
        line[j] = '\0';
        for(j = 0, k = 0; line[j] != ' '; j++)
        {
            ch[k] = line[j];
            k++;
        }
        ch[k] = '\0';

        if(instructionSet.find(ch) == instructionSet.end())
        {
            number++;
            CreateNewLabel();
            while(line[k] == ' ') k++;
            strcpy(line, line+k);
        }
        for(j = 0, k = 0; line[j] != ' '; j++)
        {
            ch[k] = line[j];
            k++;
        }
        ch[k] = '\0';
        if(strcmp(".BLKW", ch) == 0)
        {
            j++;
            while(line[j] == ' ') j++;
            j++;
            char m[50];
            int n = 0;
            while(line[j] != ' ' && line[j] != '\0')
            {
                m[n] = line[j];
                n++;
                j++;
            }
            m[n] = '\0';
            addnumber = DecimaltoBin(m);
        }
        else if(strcmp(".STRINGZ", ch) == 0)
        {
            j++;
            while(line[j] == ' ') j++;
            j++;
            char m[50];
            int n = 0;
            while(line[j] != '"' && line[j] != '\0')
            {
                m[n] = line[j];
                n++;
                j++;
            }
            m[n] = '\0';
            addnumber = strlen(m) + 1;

        }
        else addnumber = 1;
        
        SaveLine();
        // puts(line);
        // puts(end1);
        if(strcmp(end1, line) == 0) /* while .END , end the loop */
        break;
    }
}

/*Create a new node to save the information of a label*/
void CreateNewLabel()
{
    char labelname[20];
    int i = 0;
    for(i = 0; line[i] != ' '; i++)
    {
        labelname[i] = line[i];
    }
    labelname[i] = '\0';
    p1 = (struct label*)malloc(sizeof(struct label));
    strcpy(p1->name, labelname);
    p1->location = linenumber;
    if(number == 1)
    {
        head = p1;
        p1->next = NULL;
        p2 = p1;
    }
    else
    {
        p2->next = p1;
        p1->next = NULL;
        p2 = p1;
    }
}

/*save a line in link*/
void SaveLine()
{
    pr1 = (struct linecontent*)malloc(sizeof(struct linecontent));
    strcpy(pr1->linechar, line);
    pr1->no = linenumber;
    if(linenumber == 0)
    {
        firstline = pr1;
        pr1->next = NULL;
        pr2 = pr1;
    }
    else
    {
        pr2->next = pr1;
        pr1->next = NULL;
        pr2 = pr1;
    }
}

/*function PrintfRegister, translate decimal into 3-bit binary code to print*/
void PrintfRegister(char reg)
{
    int x = reg - '0';
    cout << bitset<3>(x);
}
/*Translate assembly code into binary machine code*/
void Translate()
{
    int i;
    char r[100]; /*r[100] acts as a temporary register */
    strcpy(r, pr->linechar);
    switch(r[0])
    {
        case 'A': A(r, i); break;
        case 'N': N(r, i); break;
        case 'L': L(r, i); break;
        case 'S': S(r, i); break;
        case 'T': T(r, i); break;
        case 'B': B(r, i); break;
        case 'J': J(r, i); break;
        case 'R': R(r, i); break;
        case 'H': H(r, i); break;
        case '.': Point(r, i); break;
        case 'G': G(r); break;
        case 'O': O(r); break;
        case 'P': P(r); break;
        case 'I': I(r); break;
    }
}

void A(char *a, int i) /* AND and ADD */
{
    if(a[i+1] == 'D') /*ADD*/
    {
        printf("0001");
        i = i + 3;
        while(a[i] == ' ') i++;
        /*print register*/
        PrintfRegister(a[i+1]);
        PrintfRegister(a[i+5]);
        if(a[i+8] == '#') /*immediate decimal/hex number*/
        {
            printf("1");
            i = i + 8;
            int k = 0;
            int j = 0;
            int imm;
            char decimal[10];
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            imm = DecimaltoBin(decimal);
            cout << bitset<5>(imm) << endl;
        }
        else if(a[i+8] == 'R') /* register */
        {
            printf("000");
            PrintfRegister(a[i+9]);
            printf("\n");
        }
        else if(a[i+8] == 'x') /* hex imm */
        {
            printf("1");
            int imm;
            char hex[20];
            i = i + 9;
            int k = 0;
            while(a[i] != '\0' && a[i] != ' ')
            {
                hex[k] = a[i];
                i++;
                k++;
            }
            hex[k] = '\0';
            imm = Hex(hex);
            cout << bitset<5>(imm) << endl;
        }
    }

    else /*AND*/
    {
        printf("0101");
        i = i + 3;  
        while(a[i] == ' ') i++;
        /*print register*/
        PrintfRegister(a[i+1]);
        PrintfRegister(a[i+5]);
        if(a[i+8] == '#') /*immediate decimal/hex number*/
        {
            printf("1");
            i = i + 8;
            int k = 0;
            int j = 0;
            int imm;
            char decimal[10];
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            imm = DecimaltoBin(decimal);
            cout << bitset<5>(imm) << endl;
        }
        else if(a[i+8] == 'R') 
        {
            printf("000");
            PrintfRegister(a[i+9]);
            printf("\n");
        }
        else if(a[i+8] == 'x') /* hex imm */
        {
            printf("1");
            int imm;
            char hex[20];
            i = i + 9;
            int k = 0;
            while(a[i] != '\0' && a[i] != ' ')
            {
                hex[k] = a[i];
                i++;
                k++;
            }
            hex[k] = '\0';
            imm = Hex(hex);
            cout << bitset<5>(imm) << endl;
        } 
    }
}
void N(char *a, int i) /* NOT */
{
    printf("1001");
    i = i + 3;
    while(a[i] == ' ') i++;    
    PrintfRegister(a[i+1]);
    PrintfRegister(a[i+5]);
    printf("111111\n");
}
void L(char *a, int i) /* LD LDR LDI LEA*/
{
    if(a[i+2] == ' ') /*LD*/
    {
        printf("0010");
        i = i + 2;
        while(a[i] == ' ') i++;
        int offset;
        char labeloff[20];
        PrintfRegister(a[i+1]);
        if(a[i+4] == '#') /* offset is a decimal number */
        {
            i = i + 4;
            int k=0, j = 0;
            char decimal[20];
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            offset = DecimaltoBin(decimal);
        }
        else /* offset is a label */
        {
            int k;
            int j = 0;
            // printf("%c\n",a[i+4]);
            for(k = i+4; a[k] != ' ' && a[k] != '\0'; k++)
            {
                labeloff[j] = a[k];
                j++;
            }
            labeloff[j] = '\0';
            struct label *p = head;
            // printf("%c\n", labeloff[0]);
            while(1)
            {
                if(strcmp(p->name, labeloff) == 0)
                    break;
                p = p->next;
            }
            // printf("\n%d %d\n", p->location, currentline);
            offset = (p->location) - currentline - 1;
            // printf("%d\n", offset);
        }
        cout << bitset<9>(offset) << endl;
    }
    else if(a[i+2] == 'R') /*LDR*/
    {
        printf("0110");
        i = i + 3;
        while(a[i] == ' ') i++;
        PrintfRegister(a[i+1]);
        PrintfRegister(a[i+5]);
        // printf("%c\n", a[i+9]);
        int imm;
        if(a[i+8] == '#')
        {
            i = i + 8;
            int k;
            int j = 0;
            char decimal[10];
            k = 0;
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            // puts(decimal);
            imm = DecimaltoBin(decimal);
            cout << bitset<6>(imm) << endl;    
            
        }
        else if(a[i+8] == 'x') /* hex imm */
        {
            int imm;
            char hex[20];
            i = i + 9;
            int k = 0;
            while(a[i] != '\0' && a[i] != ' ')
            {
                hex[k] = a[i];
                i++;
                k++;
            }
            hex[k] = '\0';
            imm = Hex(hex);
            cout << bitset<6>(imm) << endl;
        }   

    }
    else if(a[i+2] == 'I') /*LDI*/
    {
        printf("1010");
        i = i + 3;
        while(a[i] == ' ') i++;
        int offset;
        char labeloff[20];
        PrintfRegister(a[i+1]);
        if(a[i+4] == '#') /* offset is a decimal number */
        {
            i = i + 4;
            int k=0, j = 0;
            char decimal[20];
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            offset = DecimaltoBin(decimal);
        }
        else /* offset is a label */
        {
            int k;
            int j = 0;
            for(k = i+4; a[k] != ' ' && a[k] != '\0'; k++)
            {
                labeloff[j] = a[k];
                j++;
            }
            labeloff[j] = '\0';
            struct label *p = head;
            while(1)
            {
                if(strcmp(p->name, labeloff) == 0)
                    break;
                p = p->next;
            }
            // printf("\n%d %d\n", p->location, currentline);
            offset = (p->location) - currentline - 1;
        }
        // printf("\n%d\n", offset);
        cout << bitset<9>(offset) << endl;
    }
    else if(a[i+2] == 'A') /*LEA*/
    {
        printf("1110");
        i = i + 3;
        while(a[i] == ' ') i++;
        int offset;
        char labeloff[20];
        PrintfRegister(a[i+1]);
        if(a[i+4] == '#') /* offset is a decimal number */
        {
            i = i + 4;
            int k = 0, j = 0;
            char decimal[20];
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            offset = DecimaltoBin(decimal);
        }
        else /* offset is a label */
        {
            int k;
            int j = 0;
            for(k = i+4; a[k] != ' ' && a[k] != '\0'; k++)
            {
                labeloff[j] = a[k];
                j++;
            }
            labeloff[j] = '\0';
            struct label *p = head;
            while(1)
            {
                if(strcmp(p->name, labeloff) == 0)
                    break;
                p = p->next;
            }
            // printf("\n%d %d\n", p->location, currentline);
            offset = (p->location) - currentline - 1;
        }
        // printf("\n%d\n", offset);
        cout << bitset<9>(offset) << endl;
    }
}
void S(char *a, int i) /* ST STR STI */
{
    if(a[i+2] == ' ') /* ST */
    {
        printf("0011");
        i = i + 2;
        while(a[i] == ' ') i++;
        int offset;
        char labeloff[20];
        PrintfRegister(a[i+1]);
        if(a[i+4] == '#') /* offset is a decimal number */
        {
            i = i + 4;
            int k=0, j = 0;
            char decimal[20];
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            offset = DecimaltoBin(decimal);
        }
        else /* offset is a label */
        {
            int k;
            int j = 0;
            for(k = i+4; a[k] != ' ' && a[k] != '\0'; k++)
            {
                labeloff[j] = a[k];
                j++;
            }
            labeloff[j] = '\0';
            struct label *p = head;
            while(1)
            {
                if(strcmp(p->name, labeloff) == 0)
                    break;
                p = p->next;
            }
            // printf("\n%d %d\n", p->location, currentline);
            offset = (p->location) - currentline - 1;
        }
        //  printf("\n%d\n", offset);
        cout << bitset<9>(offset) << endl;        
    }
    else if(a[i+2] == 'R') /* STR */
    {
        printf("0111");
        i = i + 3;
        while(a[i] == ' ') i++;
        PrintfRegister(a[i+1]);
        PrintfRegister(a[i+5]);
        int imm;
        if(a[i+8] == '#')
        {
            i = i + 8;
            int k;
            int j = 0;
            char decimal[10];
            k = 0;
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            // puts(decimal);
            imm = DecimaltoBin(decimal);
            cout << bitset<6>(imm) << endl;    
            
        }
        else if(a[i+8] == 'x') /* hex imm */
        {
            int imm;
            char hex[20];
            i = i + 9;
            int k = 0;
            while(a[i] != '\0' && a[i] != ' ')
            {
                hex[k] = a[i];
                i++;
                k++;
            }
            hex[k] = '\0';
            imm = Hex(hex);
            cout << bitset<6>(imm) << endl;
        }      
    }
    else if(a[i+2] == 'I') /* STI */
    {
        printf("1011");
        i = i + 3;
        while(a[i] == ' ') i++;
        int offset;
        char labeloff[20];
        PrintfRegister(a[i+1]);
        if(a[i+4] == '#') /* offset is a decimal number */
        {
            i = i + 4;
            int k=0, j = 0;
            char decimal[20];
            i++;
            while(a[i] != '\0' && a[i] != ' ')
            {
                decimal[k] = a[i];
                k++;
                i++;
            }
            decimal[k] = '\0';
            offset = DecimaltoBin(decimal);
        }
        else /* offset is a label */
        {
            int k;
            int j = 0;
            for(k = i+4; a[k] != '\0'; k++)
            {
                labeloff[j] = a[k];
                j++;
            }
            labeloff[j] = '\0';
            struct label *p = head;
            while(1)
            {
                if(strcmp(p->name, labeloff) == 0)
                    break;
                p = p->next;
            }
            // printf("\n%d %d\n", p->location, currentline);
            offset = (p->location) - currentline - 1;
        }
        // printf("\n%d\n", offset);
        cout << bitset<9>(offset) << endl;        
    }
}
void T(char *a, int i) /* TRAP */
{
    printf("11110000");
    i = i + 4;
    while(a[i]  == ' ') i++;
    i++;
    while(a[i] == '0') i++;
    if(a[i+1] == '\0' || a[i+1] == ' ') /* 1-bit hexadecimal number */
    {
        printf("0000");
        int vect;
        if('0' <= a[i] && a[i] <= '9') vect = a[i] - 48;
        else vect = a[i] - 'A' + 10;
        cout << bitset<4>(vect) << endl;
    }
    else /* 2-bit hexadecial number */
    {
        int vect;
        if('0' <= a[i] && a[i] <= '9') vect = a[i] - 48;
        else vect = a[i] - 'A' + 10;
        cout << bitset<4>(vect);
        if('0' <= a[i+1] && a[i+1] <= '9') vect = a[i+1] - 48;
        else vect = a[i+1] - 'A' + 10;
        cout << bitset<4>(vect) << endl;
    }
}
void B(char *a, int i) /* BR */
{
    printf("0000");
    if(a[i+2] == 'n' && a[i+3] == 'z' && a[i+4] == 'p') 
    {
        printf("111");
        i = i + 5;
    }
    else if(a[i+2] == 'n' && a[i+3] == 'z')
    {
        printf("110");
        i = i + 4;
    }
    else if(a[i+2] == ' ')
    {
        printf("111");
        i = i + 2;
    }
    else if(a[i+2] == 'z' && a[i+3] == 'p')
    {
        printf("011");
        i = i + 4;
    }
    else if(a[i+2] == 'n' && a[i+3] == 'p') 
    { 
        printf("101");
        i = i + 4;
    }
    else if(a[i+2] == 'n')
    {
        printf("100");
        i = i + 3;
    }
    else if(a[i+2] == 'z')
    {
        printf("010");
        i = i + 3;
    }
    else if(a[i+2] == 'p')
    {
        printf("001");
        i = i + 3;
    }

    int offset;
    char labeloff[20];
    while(a[i] == ' ') i++;
    if(a[i] == '#') /* offset is a decimal number */
    {
        int k = 0;
        int j = 0;
        char decimal[10];
        i++;
        while(a[i] != '\0' && a[i] != ' ')
        {
            decimal[k] = a[i];
            k++;
            i++;
        }
        decimal[k] = '\0';
        offset = DecimaltoBin(decimal);
    }
    else /* offset is a label */
    {
        int k;
        int j = 0;
        for(k = i; a[k] != ' ' && a[k] != '\0'; k++)
        {
            labeloff[j] = a[k];
            j++;
        }
        labeloff[j] = '\0';
        struct label *p = head;
        while(1)
        {
            if(strcmp(p->name, labeloff) == 0)
                break;
            p = p->next;
        }
        // printf("\n%d %d\n", p->location, currentline);
        offset = (p->location) - currentline - 1;
    }
    // printf("\n%d\n", offset);
    cout << bitset<9>(offset) << endl;
}
void J(char *a, int i) /* JMP JSR */
{
    if(a[i+1] == 'M') /* JMP */
    {
        printf("1100000");
        PrintfRegister(a[i+9]);
        printf("000000\n");
    }
    else if(a[i+1] == 'S') /* JSR or JSRR*/
    {
        if(a[i+3] == 'R') /* JSRR */
        {
            printf("0100000");
            i = i + 4;
            while(a[i] == ' ') i++;
            PrintfRegister(a[i+1]);
            printf("000000\n");
        }
        else /* JSR */
        {
            printf("01001");
            i = i + 3;
            int k;
            int j = 0;
            int offset;
            char labeloff[20];        
            while(a[i] == ' ') i++;
            if(a[i] == '#')
            {
                char decimal[10];
                k = 0;
                i++;
                while(a[i] != '\0' && a[i] != ' ')
                {
                    decimal[k] = a[i];
                    k++;
                    i++;
                }
                decimal[k] = '\0';
                // puts(decimal);
                offset = DecimaltoBin(decimal);
            }
            else
            {
                for(k = i; a[k] != ' ' && a[k] != '\0'; k++)
                {
                    labeloff[j] = a[k];
                    j++;
                }
                labeloff[j] = '\0';
                struct label *p = head;
                while(1)
                {
                    if(strcmp(p->name, labeloff) == 0)
                        break;
                    p = p->next;
                }
            // printf("\n%d %d\n", p->location, currentline);
                offset = (p->location) - currentline - 1;
            }
            cout << bitset<11>(offset) << endl;
        }

    }
}
void R(char *a, int i) /* RTI and RET */
{
    if(a[i+1] == 'E') /* RET */
        printf("1100000111000000\n");
    else
        printf("1000000000000000\n");
}
void Point(char *a, int i)
{
    switch(a[i+1])
    {
        case 'O': orig(a); break;
        case 'F': Fill(a); break;
        case 'B': blkw(a); break;
        case 'S': stringz(a); break;
    }
}
void H(char *a, int i) /* HALT */
{
    printf("1111000000100101\n");
}
void stringz(char *a)
{
    int i = 0;
    i = i + 8;
    while(a[i] != ' ') i++;
    i++;
    char str[50];
    int j = 0, k = 0;
    i++;
    while(a[i] != '"')
    {
        str[k] = a[i];
        i++;
        k++;
    }
    str[k] = '\0';
    StringPrint(str);
}
void orig(char *a)
{
    int i = 0;
    i = i + 5;
    while(a[i] == ' ') i++;
    char vect[8];
    int k = 0;
    i++;
    char op = a[i-1];
    while(a[i] != '\0' && a[i] != ' ')
    {
        vect[k] = a[i];
        i++;
        k++;
    }
    vect[k] = '\0';
    if(op == 'x')
        HextoBin(vect, 4);
}
void Fill(char *a) /* .FILL */
{
    int i = 0;
    i = i + 5;
    while(a[i] == ' ') i++;
    char imm[8];
    int k = 0;
    i++;
    char op = a[i-1];
    while(a[i] != ' ' && a[i] != '\0')
    {
        imm[k] = a[i];
        i++;
        k++;
    }
    imm[k] = '\0';
    if(op == 'x')
    {
        int result;
        result = Hex(imm);
        cout << bitset<16>(result) << endl;
    }
    else/* op == '#' */
    {
        int result;
        result = DecimaltoBin(imm);
        cout << bitset<16>(result) << endl;
    }

}
void blkw(char *a) /* .BLKW */
{
    int i = 0;
    i = i + 5;
    while(a[i] == ' ') i++;
    i++;
    int k = 0;
    char unsi[20];
    while(a[i] != '\0' && a[i] != ' ')
    {
        unsi[k] = a[i];
        i++;
        k++;
    }
    unsi[k] = '\0';
    int result = DecimaltoBin(unsi);
    int j = 0;
    for(j = 0; j < result; j++)
        printf("0111011101110111\n");
}
void G(char *a)/* GETC, trap x20 */
{
    printf("1111000000100000\n");
}
void O(char *a) /* OUT, trap x21 */
{
    printf("1111000000100001\n");
}
void P(char *a) /* PUTS, trap x22 / PUTSP, trap x24 */
{
    int i = 0;
    if(a[i+4] == 'P') /* PUTSP */
        printf("1111000000100100\n");
    else /* puts */
        printf("1111000000100010\n");
}
void I(char *a) /* IN, trap x23  */
{
    printf("1111000000100011\n");
}
void HextoBin(char *vect, int bitnumber)
{
    if(strlen(vect) == 1)
    {
        printf("000000000000");
        PrintBitset(vect[0]);
        printf("\n");
    }
    else if(strlen(vect) == 2)
    {
        printf("00000000");
        PrintBitset(vect[0]);
        PrintBitset(vect[1]);
        printf("\n");
    }
    else if(strlen(vect) == 3)
    {
        printf("0000");
        PrintBitset(vect[0]);
        PrintBitset(vect[1]);
        PrintBitset(vect[2]);
        printf("\n");
    }
    else
    {
        PrintBitset(vect[0]);
        PrintBitset(vect[1]);
        PrintBitset(vect[2]);
        PrintBitset(vect[3]);
        printf("\n");
    }

}
int DecimaltoBin(char *decimal)
{
    int length = strlen(decimal);
    int i, k;
    k = 0;
    int result = 0;
    if(decimal[0] != '-')
    {
	    for(i = length; i > 0; i--)
	    {
	        result = result + pow(10, i - 1) * (decimal[k] - 48);
	        k++;
	    }
	}
	else
	{
		length--;
		for(i = length; i > 0; i--)
	    {
	        result = result + pow(10, i - 1) * (decimal[k+1] - 48);
	        k++;
	    }
	    result = - result;
	}
    return result;
}
void PrintBitset(char a)
{
    if('0' <= a && a <= '9')
        cout << bitset<4>(a - 48);
    else cout << bitset<4>(a - 'A' + 10);
}
void StringPrint(char *str)
{
    int i = 0;
    for(i = 0; str[i] != '\0'; i++)
    {
        cout << bitset<16>(str[i]) << endl;
    }
    printf("0000000000000000\n");
}
int Hex(char *hex)
{
    int result = 0;
    int length = strlen(hex);
    int i, k;
    k = 0;
    // puts(hex);
    if(hex[0] == '-') /* it is a negative hex imm */
    {
        length--;
        for(i = length; i > 0; i--)
        {
            if('0' <= hex[k+1] && hex[k+1] <= '9')
                result = result + pow(16, i - 1) * (hex[k+1] - 48);
            else 
                result = result + pow(16, i - 1) * (hex[k+1] - 'A' + 10);
            k++;
        }
        result = -result;
    }
    else
    {
        for(i = length; i > 0; i--)
        {
            if('0' <= hex[k] && hex[k] <= '9')
                result = result + pow(16, i - 1) * (hex[k] - 48);
            else 
                result = result + pow(16, i - 1) * (hex[k] - 'A' + 10);
            k++;
        }
    }
    return result;
}

