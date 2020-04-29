#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) ((a)->lenght < (b)->lenght ? (a) : (b))

typedef struct vertex{
    int x;
    int y;
    int lenght;
    char type;
    struct vertex *before;
    char ok;
}VERTEX;

typedef struct heapInfo{
    int elements;
    VERTEX *heap[10000];
}HEAPINFO;

void push(VERTEX *newVertex, HEAPINFO *heapinfo){
    heapinfo->elements++;
    int elements = heapinfo->elements;
    heapinfo->heap[elements] = newVertex;
    if(elements == 1){
        return;
    }
    while((elements/2 > 0) && newVertex->lenght < heapinfo->heap[elements / 2]->lenght){
        heapinfo->heap[elements] = heapinfo->heap[elements/2];
        heapinfo->heap[elements/2] = newVertex;
        elements /= 2;
    }
}

VERTEX *pop(HEAPINFO *heapinfo){
    VERTEX *poped = heapinfo->heap[1], *node, *min;
    if(poped == NULL){
        return NULL;
    }
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

    if((i == elements) && (node->lenght > (min = heapinfo->heap[i])->lenght)){
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

VERTEX ***createMapNode(int n, int m, char **mapa, int *princess, VERTEX***princessMap, VERTEX**dragon){
    VERTEX ***mapNode = (VERTEX***)malloc(n * sizeof(VERTEX**));
    for(int i = 0; i < n; i++) {
        mapNode[i] = (VERTEX**)malloc(m * sizeof(VERTEX*));
        for (int j = 0; j < m; j++) {
            mapNode[i][j] = (VERTEX *) malloc(sizeof(VERTEX));
            mapNode[i][j]->x = i;
            mapNode[i][j]->y = j;
            mapNode[i][j]->lenght = 2147483647;
            mapNode[i][j]->before = NULL;
            mapNode[i][j]->type = mapa[i][j];
            mapNode[i][j]->ok = 0;
            if(mapa[i][j] == 'N'){
                mapNode[i][j]->ok = 1;
            }
            if(mapa[i][j] == 'P'){
                (*princessMap)[*princess] = (VERTEX*) malloc(sizeof(VERTEX));
                (*princessMap)[*princess]->x = i;
                (*princessMap)[*princess]->y = j;
                (*princessMap)[*princess]->ok = 0;
                (*princessMap)[*princess]->type = 'P';
                (*princessMap)[*princess]->lenght = 214748647;
                (*princessMap)[*princess]->before = NULL;
                (*princess)++;
            }
            if(mapa[i][j] == 'D'){
                (*dragon)->x = i;
                (*dragon)->y = j;
                (*dragon)->lenght = 2147483647;
                (*dragon)->before = NULL;
                (*dragon)->type = mapa[i][j];
                (*dragon)->ok = 0;
            }
        }
    }
    return mapNode;
}

void printMap(VERTEX ***vertexMap, int n, int m){
    for(int i = 0; i < n; i++){
        for (int j = 0; j < m; ++j) {
            printf("%c",vertexMap[i][j]->type);
        }
        printf("\n");
    }
}

int gain(VERTEX ***vertexMap, int x, int y){
    char type = vertexMap[x][y]->type;
    if ((type == 'C') || (type == 'P') || type == 'D') return 1;
    if (type == 'H') return 2;
    return 0;
}

VERTEX* dijkstra(VERTEX ***vertexMap,VERTEX* start, VERTEX* finish, HEAPINFO *heapinfo, int m, int n){
    push(start, heapinfo);
    while(finish->ok == 0){
        VERTEX *min = pop(heapinfo);
        if(min == NULL){
            return NULL;
        }
        min->ok = 1;
        int x = min->x;
        int y = min->y;
        if(min->type == finish->type){
            finish->ok = 1;
        }
        //relax
        if((y + 1 < m) && (vertexMap[x][y + 1]->ok == 0)){        //doprava
            int plus = 0;
            plus = gain(vertexMap, x, y + 1);
            if((vertexMap[x][y + 1]->lenght > min->lenght + plus)){
                vertexMap[x][y + 1]->lenght = min->lenght + plus;
                vertexMap[x][y + 1]->before = min;
                push(vertexMap[x][y + 1],heapinfo);
            }
        }
        if((x + 1 < n) && (vertexMap[x + 1][y]->ok == 0)){        //dole
            int plus = 0;
            plus = gain(vertexMap, x + 1, y);
            if((vertexMap[x + 1][y]->lenght > min->lenght + plus)){
                vertexMap[x + 1][y]->lenght = min->lenght + plus;
                vertexMap[x + 1][y]->before = min;
                push(vertexMap[x + 1][y],heapinfo);
            }
        }
        if((y - 1 >= 0) && (vertexMap[x][y - 1]->ok == 0)){        //dolava
            int plus = 0;
            plus = gain(vertexMap, x, y - 1);
            if((vertexMap[x][y - 1]->lenght > min->lenght + plus)){
                vertexMap[x][y - 1]->lenght = min->lenght + plus;
                vertexMap[x][y - 1]->before = min;
                push(vertexMap[x][y - 1],heapinfo);
            }
        }
        if((x - 1 >= 0) && (vertexMap[x - 1][y]->ok == 0)){        //hore
            int plus = 0;
            plus = gain(vertexMap, x - 1, y);
            if((vertexMap[x - 1][y]->lenght > min->lenght + plus)){
                vertexMap[x - 1][y]->lenght = min->lenght + plus;
                vertexMap[x - 1][y]->before = min;
                push(vertexMap[x - 1][y],heapinfo);
            }
        }
    }
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    VERTEX ***vertexMap, **princessMap, *dragon;
    int princess = 0;
    dragon = (VERTEX*) malloc(sizeof(VERTEX));
    princessMap = (VERTEX**) malloc(5 * sizeof(VERTEX*));
    vertexMap = createMapNode(n, m, mapa, &princess, &princessMap, &dragon);                //inicializacia, pole princezien, drak, mapa vrcholov s údajmi

    HEAPINFO *heapinfo = malloc(sizeof(HEAPINFO));
    heapinfo->elements = 0;
    vertexMap[0][0]->ok = 1;
    if(vertexMap[0][0]->type == 'C'){
        vertexMap[0][0]->lenght = 1;
    }
    else if (vertexMap[0][0]->type == 'H'){
        vertexMap[0][0]->lenght = 2;
    }
    dijkstra(vertexMap,vertexMap[0][0],dragon,heapinfo,m,n);


    int *road = malloc(10000 * sizeof(int));
    VERTEX *temp = vertexMap[dragon->x][dragon->y];
    while(temp != NULL){
        printf("%d %d\n",temp->y, temp->x);
        temp = temp->before;
    }
    return NULL;
}


int main()
{
    char **mapa;
    int i, test, dlzka_cesty, cas, *cesta;
    int n=0, m=0, t=0;
    FILE* f;
//    while(1){
//        printf("Zadajte cislo testu (0 ukonci program):\n");
//        scanf("%d",&test);
        dlzka_cesty = 0;
        n=m=t=0;
//        switch(test){
//            case 0://ukonci program
//                return 0;
//            case 1://nacitanie mapy zo suboru
//                f=fopen("test.txt","r");
//                if(f)
//                    fscanf(f, "%d %d %d", &n, &m, &t);
//                else
//                    continue;
//                mapa = (char**)malloc(n*sizeof(char*));
//                for(i=0; i<n; i++){
//                    mapa[i] = (char*)malloc(m*sizeof(char));
//                    for (int j=0; j<m; j++){
//                        char policko = fgetc(f);
//                        if(policko == '\n') policko = fgetc(f);
//                        mapa[i][j] = policko;
//                    }
//                }
//                fclose(f);
//                cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
//                break;
//            case 2://nacitanie preddefinovanej mapy
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
//                break;
//            case 3: //pridajte vlastne testovacie vzorky
//            default:
//                continue;
//        }
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
//    }
    return 0;
}