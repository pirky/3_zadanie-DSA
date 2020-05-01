#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a,b) ((a)->length < (b)->length ? (a) : (b))

typedef struct vertex{
    int x;
    int y;
    int length;
    char type;
    struct vertex *before;
    char ok;
}VERTEX;

typedef struct heapInfo{
    int elements;
    VERTEX *heap[300];
}HEAPINFO;

typedef struct mapInfo{
    int m;
    int n;
    int princess;
    int characters[6][2];
    VERTEX ***vertexMap;
}MAPINFO;

typedef struct matrixNode{
    int prize;
    int length;
    int *path;
}MATRIXNODE;

void push(VERTEX *newVertex, HEAPINFO *heapinfo){
    heapinfo->elements++;
    int elements = heapinfo->elements;
    heapinfo->heap[elements] = newVertex;
    if(elements == 1){
        return;
    }
    while((elements/2 > 0) && (newVertex->length < heapinfo->heap[elements / 2]->length)){
        heapinfo->heap[elements] = heapinfo->heap[elements/2];
        heapinfo->heap[elements/2] = newVertex;
        elements /= 2;
    }
}

VERTEX *pop(HEAPINFO *heapinfo){
    VERTEX *poped = heapinfo->heap[1], *vertex, *min;
    if(poped == NULL){
        return NULL;
    }
    int  i = 1, elements = heapinfo->elements;
    vertex = heapinfo->heap[1] = heapinfo->heap[elements];
    while(((i *= 2) + 1 <= elements) && (vertex->length > (min = MIN(heapinfo->heap[i], heapinfo->heap[i + 1]))->length)){
        if(min == heapinfo->heap[i]){
            heapinfo->heap[i] = vertex;
            heapinfo->heap[i / 2] = min;
        }
        if(min == heapinfo->heap[i + 1]){
            heapinfo->heap[i + 1] = vertex;
            heapinfo->heap[i / 2] = min;
            i++;
        }
    }

    heapinfo->heap[elements] = NULL;
    heapinfo->elements--;
    return poped;
}

void printHeap(HEAPINFO *heapinfo){
    for(int i = 1; i <= heapinfo->elements; i++){
        printf("%d, ", heapinfo->heap[i]->length);
    }
    printf("\n");
}

VERTEX ***createMapNode(MAPINFO *mapInfo,char **mapa){
    VERTEX ***vertexMap = (VERTEX***)malloc(mapInfo->n * sizeof(VERTEX**));
    mapInfo->princess = 0;
    for(int i = 0; i < mapInfo->n; i++) {
        vertexMap[i] = (VERTEX**)malloc(mapInfo->m * sizeof(VERTEX*));
        for (int j = 0; j < mapInfo->m; j++) {
            vertexMap[i][j] = (VERTEX *) malloc(sizeof(VERTEX));
            vertexMap[i][j]->x = i;
            vertexMap[i][j]->y = j;
            vertexMap[i][j]->length = 2147483647;
            vertexMap[i][j]->before = NULL;
            vertexMap[i][j]->type = mapa[i][j];
            vertexMap[i][j]->ok = 0;
            if(mapa[i][j] == 'N'){
                vertexMap[i][j]->ok = 1;
                vertexMap[i][j]->length = 0;
            }
            if(mapa[i][j] == 'P'){
                mapInfo->princess++;
                mapInfo->characters[mapInfo->princess][0] = i;
                mapInfo->characters[mapInfo->princess][1] = j;
            }
            if(mapa[i][j] == 'D'){
                mapInfo->characters[0][0] = i;                          //x-ova súradnica
                mapInfo->characters[0][1] = j;                          //y-ova súradnica
            }
        }
    }
    return vertexMap;
}

void printMap(VERTEX ***vertexMap, int n, int m){
    for(int i = 0; i < n; i++){
        for (int j = 0; j < m; ++j) {
            printf("%3d",vertexMap[i][j]->length);
        }
        printf("\n");
    }
}

int typeSize(VERTEX ***vertexMap, int x, int y){
    char type = vertexMap[x][y]->type;
    if ((type == 'C') || (type == 'P') || type == 'D') return 1;
    if (type == 'H') return 2;
    return 0;
}

VERTEX* dijkstra(MAPINFO *mapInfo, VERTEX* start, VERTEX* finish, HEAPINFO *heapinfo){
    mapInfo->vertexMap[start->x][start->y]->length = typeSize(mapInfo->vertexMap,start->x,start->y);
    push(start, heapinfo);
    while(1){
        VERTEX *min = pop(heapinfo);
        if(min == finish){
            return finish;
        }
        min->ok = 1;
        int x = min->x;
        int y = min->y;
        if((y + 1 < mapInfo->m) && (mapInfo->vertexMap[x][y + 1] != min->before) && (mapInfo->vertexMap[x][y + 1]->ok == 0)){        //doprava
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x, y + 1);
            if((mapInfo->vertexMap[x][y + 1]->length > min->length + plus)){
                mapInfo->vertexMap[x][y + 1]->length = min->length + plus;
                mapInfo->vertexMap[x][y + 1]->before = min;
                push(mapInfo->vertexMap[x][y + 1], heapinfo);
            }

        }
        if((x + 1 < mapInfo->n) && (mapInfo->vertexMap[x + 1][y] != min->before) && (mapInfo->vertexMap[x + 1][y]->ok == 0)){        //dole
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x + 1, y);
            if((mapInfo->vertexMap[x + 1][y]->length > min->length + plus)){
                mapInfo->vertexMap[x + 1][y]->length = min->length + plus;
                mapInfo->vertexMap[x + 1][y]->before = min;
                push(mapInfo->vertexMap[x + 1][y], heapinfo);
            }

        }
        if((y - 1 >= 0) && (mapInfo->vertexMap[x][y - 1] != min->before) && (mapInfo->vertexMap[x][y - 1]->ok == 0)){        //dolava
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x, y - 1);
            if((mapInfo->vertexMap[x][y - 1]->length > min->length + plus)){
                mapInfo->vertexMap[x][y - 1]->length = min->length + plus;
                mapInfo->vertexMap[x][y - 1]->before = min;
                push(mapInfo->vertexMap[x][y - 1], heapinfo);
            }

        }
        if((x - 1 >= 0) && (mapInfo->vertexMap[x - 1][y] != min->before) && (mapInfo->vertexMap[x - 1][y]->ok == 0)){        //hore
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x - 1, y);
            if((mapInfo->vertexMap[x - 1][y]->length > min->length + plus)){
                mapInfo->vertexMap[x - 1][y]->length = min->length + plus;
                mapInfo->vertexMap[x - 1][y]->before = min;
                push(mapInfo->vertexMap[x - 1][y], heapinfo);
            }

        }
    }
}

int pathPrize(VERTEX ***vertexMap,VERTEX *temp){
    int prize = 0;
    while(temp != NULL){
        prize += typeSize(vertexMap,temp->x,temp->y);
        temp = temp->before;
    }
    return prize;
}

int *pathMap(VERTEX *start, VERTEX *end, int length, int *path){
    path[length] = end->x;
    path[length - 1] = end->y;
    if(start == end){
        return path;
    }
    pathMap(start,end->before,length - 2,path);
}

int pathLength(VERTEX *temp){
    int length = 0;
    while(temp != NULL){
        length++;
        temp = temp->before;
    }
    return length;
}

int sumPrizes(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    int sum = matrix[0][array[0]]->prize - 1;
    for (int i = 0; i < mapInfo->princess - 1; i++){
        sum += matrix[array[i]][array[i + 1]]->prize - 1;
    }
//    printf("sumPrize = %d\n",sum);
    return sum;
}

int sumLength(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    int sum = matrix[0][array[0]]->length - 1, i;
    for (int i = 0; i < mapInfo->princess - 1; i++){
        sum += matrix[array[i]][array[i + 1]]->length - 1;
    }
//    printf("sumLength = %d\n",sum);
    return sum;
}

int *sumPath(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo, MATRIXNODE *current){
    int *path = (int*)malloc(current->length * 2 * sizeof(int)), i = 2, k = 0;
    while(i < matrix[0][array[0]]->length * 2){
        path[k] = matrix[0][array[0]]->path[i];
//        printf("%d ",path[k]);
//        if(i % 2 == 1) printf("\n");
        k++;
        i++;
    }

    for (int j = 0; j < mapInfo->princess - 1; ++j){
        int i = 2;
        while(i < matrix[array[j]][array[j + 1]]->length * 2){
            path[k] = matrix[array[j]][array[j + 1]]->path[i];
//            printf("%d ",path[k]);
//            if(i % 2 == 1) printf("\n");
            k++;
            i++;
        }
    }
    return path;
}

MATRIXNODE *createPathNode(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    MATRIXNODE *current = (MATRIXNODE*)malloc(sizeof(MATRIXNODE));
    current->prize = sumPrizes(array,matrix, mapInfo);
    current->length = sumLength(array,matrix, mapInfo);
    current->path = (int*)malloc(current->length * 2 * sizeof(int));
    current->path = sumPath(array,matrix,mapInfo,current);
    return current;
}

void bestPath(MATRIXNODE *current,MATRIXNODE **best){
    if((*best) == NULL || current->prize < (*best)->prize){
        (*best) = (MATRIXNODE*) malloc(sizeof(MATRIXNODE));
        (*best)->path = current->path;
        (*best)->length = current->length;
        (*best)->prize = current->prize;
    }
}

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void permutation(int *array, int start, int end, MATRIXNODE **best, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    if(start == end){
        MATRIXNODE *current;
//        printf("0 %d %d %d\n", array[0],array[1],array[2]);
        current = createPathNode(array, matrix, mapInfo);
        bestPath(current,best);
        return;
    }
    for(int i = start; i <= end; i++){
        swap((array + i), (array + start));
        permutation(array, start + 1, end, best, matrix, mapInfo);
        swap((array + i), (array + start));
    }
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    MAPINFO *mapInfo = (MAPINFO*)malloc(sizeof(MAPINFO));
    mapInfo->m = m;
    mapInfo->n = n;
    mapInfo->vertexMap = createMapNode(mapInfo, mapa);                //inicializacia, pole suradníc princezien a draka, mapa vrcholov s údajmi
    HEAPINFO *heapinfo = malloc(sizeof(HEAPINFO));
    heapinfo->elements = 0;

    //zabitie draka

    VERTEX *popolvar = mapInfo->vertexMap[0][0];
    VERTEX *dragon = mapInfo->vertexMap[mapInfo->characters[0][0]][mapInfo->characters[0][1]];
    dragon = dijkstra(mapInfo,popolvar,dragon,heapinfo);

    int prize = pathPrize(mapInfo->vertexMap,dragon);
    int length = pathLength(dragon);
    int *dragonPath = (int*)malloc(length * 2 * sizeof(int));
    dragonPath = pathMap(popolvar, dragon, length * 2 - 1, dragonPath);

    MATRIXNODE ***matrix = (MATRIXNODE***) malloc((mapInfo->princess + 1) * sizeof(MATRIXNODE**));      //matica susednosti princezien a draka
    for (int i = 0; i < mapInfo->princess + 1; ++i) {
        matrix[i] = (MATRIXNODE**) malloc((mapInfo->princess + 1) * sizeof(MATRIXNODE*));
        for (int j = 0; j < mapInfo->princess + 1; ++j) {
            matrix[i][j] = (MATRIXNODE*) malloc(sizeof(MATRIXNODE));
        }
    }

    //naplnenie matice, teda najdenie najkratsej cesty medzi dvoma bodmi
    for (int i = 0; i < mapInfo->princess + 1; ++i) {
        for (int j = 0; j < mapInfo->princess + 1; ++j) {
            mapInfo->vertexMap = createMapNode(mapInfo,mapa);
            heapinfo = malloc(sizeof(HEAPINFO));
            heapinfo->elements = 0;
            if(i == j) {
                matrix[i][j]->prize = 0;
                matrix[i][j]->length = 0;
                matrix[i][j]->path = NULL;
                continue;
            }
            VERTEX *start = mapInfo->vertexMap[mapInfo->characters[i][0]][mapInfo->characters[i][1]];
            VERTEX *finish = mapInfo->vertexMap[mapInfo->characters[j][0]][mapInfo->characters[j][1]];
            VERTEX *character = dijkstra(mapInfo,start,finish,heapinfo);
            int characterPrize = pathPrize(mapInfo->vertexMap,character);
            int characterLength = pathLength(character);
            int *characterPath = (int*)malloc(characterLength * 2 * sizeof(int));
            characterPath = pathMap(start, finish, characterLength * 2 - 1, characterPath);
            matrix[i][j]->prize = characterPrize;
            matrix[i][j]->length = characterLength;
            matrix[i][j]->path = characterPath;
        }
    }

    int array[6];
    MATRIXNODE *best = NULL;
    for(int i = 1; i <= mapInfo->princess; i++){
        array[i-1] = i;
    }

    permutation(array, 0, mapInfo->princess-1, &best, matrix, mapInfo);

    int oldLength = length;
    length += best->length;
    int *finalPath =(int*) malloc(length * 2* sizeof(int));
    for (int k = 0; k < oldLength*2; ++k) {
        finalPath[k] = dragonPath[k];
    }
    for (int i = oldLength * 2, j = 0; i < length * 2; ++i, j++) {
        finalPath[i] = best->path[j];
    }

    //iba po draka
    *dlzka_cesty = length;
    return finalPath;
}


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
                f=fopen("D:\\Internet toto nie je\\Skola\\2 semester\\DSA  Datove struktury a algoritmy\\Velke zadania\\3-Tretie\\zadanie3\\test.txt","r");
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
                mapa[0]=strdup("CCHCNHCCHN");
                mapa[1]=strdup("NNCCCHHCCC");
                mapa[2]=strdup("DNCCNNHHHC");
                mapa[3]=strdup("CHHHCCCCCC");
                mapa[4]=strdup("CCCCCNHHHH");
                mapa[5]=strdup("PCHCCCNNNN");
                mapa[6]=strdup("NNNNNHCCCC");
                mapa[7]=strdup("CCCCCPCCCC");
                mapa[8]=strdup("CCCNNHHHHH");
                mapa[9]=strdup("HHHPCCCCCC");
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