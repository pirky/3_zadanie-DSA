#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) ((a)->lenght < (b)->lenght ? (a) : (b))

typedef struct node{
    int x;
    int y;
    int lenght;
    char type;
    struct node *before;
    char passed;
}NODE;

typedef struct heapInfo{
    int elements;
    NODE *heap[10000];
}HEAPINFO;

void push(NODE *newNode,HEAPINFO *heapinfo){
    heapinfo->elements++;
    int elements = heapinfo->elements;
    heapinfo->heap[elements] = newNode;
    if(elements == 1){
        return;
    }
    while((elements/2 > 0) && newNode->lenght < heapinfo->heap[elements/2]->lenght){
        heapinfo->heap[elements] = heapinfo->heap[elements/2];
        heapinfo->heap[elements/2] = newNode;
        elements /= 2;
    }
}

NODE *pop(HEAPINFO *heapinfo){
    NODE *poped = heapinfo->heap[1], *node, *min;
    int  i = 1, elements = heapinfo->elements;
    node = heapinfo->heap[1] = heapinfo->heap[elements];
    while(((i *= 2) + 1 <= elements) && (node->lenght > (min = MIN(heapinfo->heap[i],heapinfo->heap[i + 1]))->lenght)){
        if(min->lenght == heapinfo->heap[i]->lenght){
            heapinfo->heap[i / 2] = min;
            heapinfo->heap[i] = node;
        }
        if(min->lenght == heapinfo->heap[i + 1]->lenght){
            heapinfo->heap[i / 2] = min;
            heapinfo->heap[i + 1] = node;
            i++;
        }
    }
    if((i == elements) && (node->lenght > (min = MIN(heapinfo->heap[i],heapinfo->heap[i + 1]))->lenght)){
        heapinfo->heap[i / 2] = min;
        heapinfo->heap[i] = node;
    }
    heapinfo->elements--;
    return poped;
}

void printHeap(HEAPINFO *heapinfo){
    for(int i = 1; i <= heapinfo->elements; i++){
        printf("%d, ", heapinfo->heap[i]->lenght);
    }
    printf("\n");
}

NODE ***createMapNode(int n, int m, char **mapa, int *princess){
    NODE ***mapNode = (NODE***)malloc(n * sizeof(NODE**));
    for(int i = 0; i < n; i++) {
        mapNode[i] = (NODE**)malloc(m * sizeof(NODE*));
        for (int j = 0; j < m; j++) {
            mapNode[i][j] = (NODE *) malloc(sizeof(NODE));
            mapNode[i][j]->x = i;
            mapNode[i][j]->y = j;
            mapNode[i][j]->lenght = 2147483647;
            mapNode[i][j]->before = NULL;
            mapNode[i][j]->type = mapa[i][j];
            mapNode[i][j]->passed = 0;
            if(mapa[i][j] == 'P'){
                (*princess)++;
            }
        }
    }
    return mapNode;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    NODE ***mapNode;
    int princess = 0;
    char road = 0;
    mapNode = createMapNode(n, m, mapa, &princess);
    HEAPINFO *heapinfo = malloc(sizeof(HEAPINFO));
    heapinfo->elements = 0;
    if(mapNode[0][0]->type == 'C'){
        mapNode[0][0]->lenght = 1;
    }
    else if (mapNode[0][0]->type == 'H'){
        mapNode[0][0]->lenght = 2;
    }
    push(mapNode[0][0],heapinfo);
    dlzka_cesty[0] = 0;
    dlzka_cesty[1] = 0;

    return NULL;
}

////test halda
//void main(){
//    HEAPINFO *heapinfo = malloc(sizeof(HEAPINFO));
//    heapinfo->elements = 0;
//    int array[12] = {20,13,14,5,6,2,1,7,9,10,8};
//    for(int i = 0; i < 11; i++){
//        NODE* node = malloc(sizeof(NODE));
//        node->lenght = array[i];
//        push(node,heapinfo);
//    }
//    printHeap(heapinfo);
//    while(heapinfo->elements > 0){
//        printf("%d, ", pop(heapinfo)->lenght);
//    }
//
//}

int main()
{
    char **mapa;
    int i, test, dlzka_cesty, cas, *cesta;
    int n=0, m=0, t=0;
    FILE* f;
    while(1){
        printf("Zadajte cislo testu (0 ukonci program):\n");
        scanf("%d",&test);
        dlzka_cesty = 0;
        n=m=t=0;
        switch(test){
            case 0://ukonci program
                return 0;
            case 1://nacitanie mapy zo suboru
                f=fopen("test.txt","r");
                if(f)
                    fscanf(f, "%d %d %d", &n, &m, &t);
                else
                    continue;
                mapa = (char**)malloc(n*sizeof(char*));
                for(i=0; i<n; i++){
                    mapa[i] = (char*)malloc(m*sizeof(char));
                    for (int j=0; j<m; j++){
                        char policko = fgetc(f);
                        if(policko == '\n') policko = fgetc(f);
                        mapa[i][j] = policko;
                    }
                }
                fclose(f);
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
            case 2://nacitanie preddefinovanej mapy
                n = 10;
                m = 10;
                t = 12;
                mapa = (char**)malloc(n*sizeof(char*));
                mapa[0]="CCHCNHCCHN";
                mapa[1]="NNCCCHHCCC";
                mapa[2]="DNCCNNHHHC";
                mapa[3]="CHHHCCCCCC";
                mapa[4]="CCCCCNHHHH";
                mapa[5]="PCHCCCNNNN";
                mapa[6]="NNNNNHCCCC";
                mapa[7]="CCCCCPCCCC";
                mapa[8]="CCCNNHHHHH";
                mapa[9]="HHHPCCCCCC";
                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
                break;
            case 3: //pridajte vlastne testovacie vzorky
            default:
                continue;
        }
        cas = 0;
        for(i=0; i<dlzka_cesty; i++){
            printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
            if(mapa[cesta[i*2+1]][cesta[i*2]] == 'H')
                cas+=2;
            else
                cas+=1;
            if(mapa[cesta[i*2+1]][cesta[i*2]] == 'D' && cas > t)
                printf("Nestihol si zabit draka!\n");
            if(mapa[cesta[i*2+1]][cesta[i*2]] == 'N')
                printf("Prechod cez nepriechodnu prekazku!\n");
            if(i>0 && abs(cesta[i*2+1]-cesta[(i-1)*2+1])+abs(cesta[i*2]-cesta[(i-1)*2])>1)
                printf("Neplatny posun Popolvara!\n");
        }
        printf("%d\n",cas);
        free(cesta);
        for(i=0; i<n; i++){
            free(mapa[i]);
        }
        free(mapa);
    }
    return 0;
}