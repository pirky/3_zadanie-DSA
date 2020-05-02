#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * makro na zistenie menšieho dieťaťa v halde
 */

#define MIN(a,b) ((a)->price < (b)->price ? (a) : (b))

/*
 * štruktúra vertex obsahuje všetky informácie o jednom vrchole v mape,
 * premenná "ok" hovorí o tom, či už bol daný vrchol prejdený
 */

typedef struct vertex{
    int x;
    int y;
    int price;
    char type;
    struct vertex *before;
    char ok;
}VERTEX;

typedef struct heapInfo{
    int elements;
    VERTEX *heap[300];
}HEAPINFO;

/*
 * štruktúra mapInfo obsahuje rozmery mapy, počet princezien, súradnice draka
 * a všetkých princezien a mapu
 */

typedef struct mapInfo{
    int m;
    int n;
    int princess;
    int characters[6][2];
    VERTEX ***vertexMap;
}MAPINFO;

typedef struct matrixNode{
    int price;
    int length;
    int *path;
}MATRIXNODE;

/*
 * funkcia push vloží do haldy daný vrchol
 */

void push(VERTEX *newVertex, HEAPINFO *heapinfo){
    heapinfo->elements++;
    int elements = heapinfo->elements;
    heapinfo->heap[elements] = newVertex;
    if(elements == 1){
        return;
    }
    while((elements/2 > 0) && (newVertex->price < heapinfo->heap[elements / 2]->price)){
        heapinfo->heap[elements] = heapinfo->heap[elements/2];
        heapinfo->heap[elements/2] = newVertex;
        elements /= 2;
    }
}

/*
 * funkcia pop vybere z haldy vrchol s najmenšou cenou cesty a vráti ho,
 * v prípade, že je halda prázdna vráti NULL
 */

VERTEX *pop(HEAPINFO *heapinfo){
    VERTEX *poped = heapinfo->heap[1], *vertex, *min;
    if(poped == NULL){
        return NULL;
    }
    int  i = 1, elements = heapinfo->elements;
    vertex = heapinfo->heap[1] = heapinfo->heap[elements];
    while(((i *= 2) + 1 <= elements) && (vertex->price > (min = MIN(heapinfo->heap[i], heapinfo->heap[i + 1]))->price)){
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

/*
 * funkcia createMap vytvorí z mapy na vstupe mapu, ktorá bude mať všetky
 * potrebné informácie o danom vrchole. Zároveň zráta počet princezien,
 * a vytvorí pole so súradnicami draka a všetkými princeznami.
 * Nakoniec túto mapu vráti.
 */

VERTEX ***createMap(MAPINFO *mapInfo, char **mapa){
    VERTEX ***vertexMap = (VERTEX***)malloc(mapInfo->n * sizeof(VERTEX**));
    mapInfo->princess = 0;
    for(int i = 0; i < mapInfo->n; i++) {
        vertexMap[i] = (VERTEX**)malloc(mapInfo->m * sizeof(VERTEX*));
        for (int j = 0; j < mapInfo->m; j++) {
            vertexMap[i][j] = (VERTEX *) malloc(sizeof(VERTEX));
            vertexMap[i][j]->x = i;
            vertexMap[i][j]->y = j;
            vertexMap[i][j]->price = 2147483647;
            vertexMap[i][j]->before = NULL;
            vertexMap[i][j]->type = mapa[i][j];
            vertexMap[i][j]->ok = 0;
            if(mapa[i][j] == 'N'){
                vertexMap[i][j]->ok = 1;
                vertexMap[i][j]->price = 0;
            }
            if(mapa[i][j] == 'P'){
                mapInfo->princess++;
                mapInfo->characters[mapInfo->princess][0] = i;
                mapInfo->characters[mapInfo->princess][1] = j;
            }
            if(mapa[i][j] == 'D'){
                mapInfo->characters[0][0] = i;
                mapInfo->characters[0][1] = j;
            }
        }
    }
    return vertexMap;
}

void printMap(VERTEX ***vertexMap, int n, int m){
    for(int i = 0; i < n; i++){
        for (int j = 0; j < m; ++j) {
            printf("%c",vertexMap[i][j]->type);
        }
        printf("\n");
    }
}

/*
 * funkcia typeSize vráti 2 ak je daný vrchol "H" - húština, inač vráti 1
 */

int typeSize(VERTEX ***vertexMap, int x, int y){
    char type = vertexMap[x][y]->type;
    if (type == 'H') return 2;
    else return 1;
}

/*
 * Dijkstrov algoritmus modifikovaný do zadania Popolvár
 */

VERTEX* dijkstra(MAPINFO *mapInfo, VERTEX* start, VERTEX* finish, HEAPINFO *heapinfo){

    /*
     * inicializácia nastaením ceny v danom počiatočnom vrchole
     * a pridanie tohto vrcholu do haldy
     */

    mapInfo->vertexMap[start->x][start->y]->price = typeSize(mapInfo->vertexMap, start->x, start->y);
    push(start, heapinfo);
    while(1){
        VERTEX *min = pop(heapinfo);
        if(min == NULL){
            return NULL;
        }
        /*
         * ak sa vrchol, ktorý vráti funkcia pop z haldy rovná cieľu,
         * tak sa ukončí funkcia, pretože prehľadávenie došlo do cieľa
         * a z neho už nie je potrebné prehľadávať mapu ďalej
         */

        if(min == finish){
            return finish;
        }
        min->ok = 1;
        int x = min->x;
        int y = min->y;

        /*
         * pridanie vrcholu do haldy napravo od polohy aktuálneho vrcholu, ak je
         * po relaxácii cena cesty menšia ako aktuálna cesta v danom vrchole,
         * respektíve ak ešte nebol daný vrchol zrelaxovaný
         */

        if((y + 1 < mapInfo->m) && (mapInfo->vertexMap[x][y + 1] != min) && (mapInfo->vertexMap[x][y + 1]->ok == 0)){
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x, y + 1);
            if((mapInfo->vertexMap[x][y + 1]->price > min->price + plus)){
                mapInfo->vertexMap[x][y + 1]->price = min->price + plus;
                mapInfo->vertexMap[x][y + 1]->before = min;
                push(mapInfo->vertexMap[x][y + 1], heapinfo);
            }
        }

        /*
         * pridanie dolného "suseda" do haldy na rovankom princípe ako je
         * opísany vyššie
         */

        if((x + 1 < mapInfo->n) && (mapInfo->vertexMap[x + 1][y] != min) && (mapInfo->vertexMap[x + 1][y]->ok == 0)){
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x + 1, y);
            if((mapInfo->vertexMap[x + 1][y]->price > min->price + plus)){
                mapInfo->vertexMap[x + 1][y]->price = min->price + plus;
                mapInfo->vertexMap[x + 1][y]->before = min;
                push(mapInfo->vertexMap[x + 1][y], heapinfo);
            }
        }

        /*
         * pridanie ľavého "suseda" do haldy na rovankom princípe ako je
         * opísany vyššie
         */

        if((y - 1 >= 0) && (mapInfo->vertexMap[x][y - 1] != min) && (mapInfo->vertexMap[x][y - 1]->ok == 0)){
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x, y - 1);
            if((mapInfo->vertexMap[x][y - 1]->price > min->price + plus)){
                mapInfo->vertexMap[x][y - 1]->price = min->price + plus;
                mapInfo->vertexMap[x][y - 1]->before = min;
                push(mapInfo->vertexMap[x][y - 1], heapinfo);
            }
        }

        /*
         * pridanie horného "suseda" do haldy na rovankom princípe ako je
         * opísany vyššie
         */

        if((x - 1 >= 0) && (mapInfo->vertexMap[x - 1][y] != min) && (mapInfo->vertexMap[x - 1][y]->ok == 0)){
            int plus = 0;
            plus = typeSize(mapInfo->vertexMap, x - 1, y);
            if((mapInfo->vertexMap[x - 1][y]->price > min->price + plus)){
                mapInfo->vertexMap[x - 1][y]->price = min->price + plus;
                mapInfo->vertexMap[x - 1][y]->before = min;
                push(mapInfo->vertexMap[x - 1][y], heapinfo);
            }
        }
    }
}

/*
 * funkcia pathPrice zistí cenu konkrétnej cesty
 */

int pathPrice(VERTEX ***vertexMap, VERTEX *temp){
    int price = 0;
    while(temp != NULL){
        price += typeSize(vertexMap, temp->x, temp->y);
        temp = temp->before;
    }
    return price;
}

/*
 * funkcia pathLength zisti dĺžku cesty
 */

int pathLength(VERTEX *temp){
    int length = 0;
    while(temp != NULL){
        length++;
        temp = temp->before;
    }
    return length;
}

/*
 * funkcia pathMap vytvorí cestu zo štartovacieho vrcholu
 * do konečného vrcholu a nakoniec ju vráti
 */

int *pathMap(VERTEX *start, VERTEX *end, int length, int *path){
    path[length] = end->x;
    path[length - 1] = end->y;
    if(start == end){
        return path;
    }
    pathMap(start,end->before,length - 2,path);
}

/*
 * funkcia sumPrices zosumarizuje ceny ciest, z ktorých sa daná
 * permutácia skladá
 */

int sumPrices(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    int sum = matrix[0][array[0]]->price - 1;
    for (int i = 0; i < mapInfo->princess - 1; i++){
        sum += matrix[array[i]][array[i + 1]]->price - 1;
    }
    return sum;
}

/*
 * funkcia sumLength zosumarizuje dĺžky ciest, z ktorých sa daná
 * permutácia skladá
 */

int sumLength(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    int sum = matrix[0][array[0]]->length - 1;
    for (int i = 0; i < mapInfo->princess - 1; i++){
        sum += matrix[array[i]][array[i + 1]]->length - 1;
    }
    return sum;
}

/*
 * funkcia sumPath spojí cesty v danej permutácií
 */

int *sumPath(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo, MATRIXNODE *current){
    int *path = (int*)malloc(current->length * 2 * sizeof(int)), i = 2, k = 0;

    while(i < matrix[0][array[0]]->length * 2){
        path[k] = matrix[0][array[0]]->path[i];
        k++;
        i++;
    }

    for (int j = 0; j < mapInfo->princess - 1; ++j){
        int iter = 2;
        while(iter < matrix[array[j]][array[j + 1]]->length * 2){
            path[k] = matrix[array[j]][array[j + 1]]->path[iter];
            k++;
            iter++;
        }
    }
    return path;
}

/*
 * funkcia createPathNode vytvorí vrchol matice z danej permutácie, ktorý
 * obsahuje sumár dĺžok ciest, cien ciest a celú cestu od draka až
 * po poslednú princeznú v danej permutácií
 */

MATRIXNODE *createPathNode(int *array, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    MATRIXNODE *current = (MATRIXNODE*)malloc(sizeof(MATRIXNODE));
    current->price = sumPrices(array, matrix, mapInfo);
    current->length = sumLength(array,matrix, mapInfo);
    current->path = (int*)malloc(current->length * 2 * sizeof(int));
    current->path = sumPath(array,matrix,mapInfo,current);
    return current;
}

/*
 * funkcia bestPath nájde cenovo najlepšiu cestu ako od draka
 * pozbierať všetky princezné
 */

void bestPath(MATRIXNODE *current,MATRIXNODE **best){
    if((*best) == NULL || current->price < (*best)->price){
        (*best) = (MATRIXNODE*) malloc(sizeof(MATRIXNODE));
        (*best)->path = current->path;
        (*best)->length = current->length;
        (*best)->price = current->price;
    }
}

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * funkcia permutation spermutuje všetky možnosti ako pozbierať princezné,
 * keď nájde nejakú permutáciu
 */

void permutation(int *array, int start, int end, MATRIXNODE **best, MATRIXNODE ***matrix, MAPINFO *mapInfo){
    if(start == end){
        MATRIXNODE *current;
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

/*
 * funkcia freeMatrix uvoľní maticu
 */

void freeMatrix(MATRIXNODE ****matrix, MAPINFO *mapInfo){
    for (int i = 0; i < mapInfo->princess + 1; ++i) {
        for (int j = 0; j < mapInfo->princess + 1; ++j) {
            free((*matrix)[i][j]);
            (*matrix)[i][j] = NULL;
        }
        free((*matrix)[i]);
        (*matrix)[i] = NULL;
    }
    free(*matrix);
    *matrix = NULL;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    MAPINFO *mapInfo = (MAPINFO*)malloc(sizeof(MAPINFO));
    mapInfo->m = m;
    mapInfo->n = n;
    mapInfo->vertexMap = createMap(mapInfo, mapa);
    HEAPINFO *heapinfo = (HEAPINFO*) malloc(sizeof(HEAPINFO));
    heapinfo->elements = 0;

    /*
     * zabitie draka
     */

    VERTEX *popolvar = mapInfo->vertexMap[0][0];
    VERTEX *dragon = mapInfo->vertexMap[mapInfo->characters[0][0]][mapInfo->characters[0][1]];
    dragon = dijkstra(mapInfo,popolvar,dragon,heapinfo);
    int price = pathPrice(mapInfo->vertexMap, dragon);

    /*
     * ak nie je možné nájsť cestu k drakovi funkcia vráti NULL
     */

    if(dragon == NULL){
        printf("Cesta neexistuje.\n");
        return NULL;
    }

    /*
     * ak drak nie je včas zabity funkcia vráti NULL
     */

    if(price > t){
        printf("Nestihol si zabiť draka.\n");
        return NULL;
    }
    int length = pathLength(dragon);
    int *dragonPath = (int*)malloc(length * 2 * sizeof(int));
    dragonPath = pathMap(popolvar, dragon, length * 2 - 1, dragonPath);

    /*
     * vytvorenie matice susednosti princezien a draka
     */

    MATRIXNODE ***matrix = (MATRIXNODE***) malloc((mapInfo->princess + 1) * sizeof(MATRIXNODE**));
    for (int i = 0; i < mapInfo->princess + 1; ++i) {
        matrix[i] = (MATRIXNODE**) malloc((mapInfo->princess + 1) * sizeof(MATRIXNODE*));
        for (int j = 0; j < mapInfo->princess + 1; ++j) {
            matrix[i][j] = (MATRIXNODE*) malloc(sizeof(MATRIXNODE));
        }
    }

    /*
     * naplnenie matice, teda najdenie najkratsej cesty medzi dvoma bodmi,
     * uloženie tejto cesty ako aj jej ceny a dĺžky
     */

    for (int i = 0; i < mapInfo->princess + 1; ++i) {
        for (int j = 0; j < mapInfo->princess + 1; ++j) {
            mapInfo->vertexMap = createMap(mapInfo, mapa);
            heapinfo = (HEAPINFO*) malloc(sizeof(HEAPINFO));
            heapinfo->elements = 0;
            if(i == j) {
                matrix[i][j]->price = 0;
                matrix[i][j]->length = 0;
                matrix[i][j]->path = NULL;
                continue;
            }
            VERTEX *start = mapInfo->vertexMap[mapInfo->characters[i][0]][mapInfo->characters[i][1]];
            VERTEX *finish = mapInfo->vertexMap[mapInfo->characters[j][0]][mapInfo->characters[j][1]];
            VERTEX *character = dijkstra(mapInfo,start,finish,heapinfo);

            /*
             * ak nie je možné nájsť cestu od draka k princeznej
             * alebo od princeznej k princeznej funkcia vráti NULL
             */

            if(character == NULL){
                printf("Cesta neexistuje.\n");
                return NULL;
            }

            int characterPrize = pathPrice(mapInfo->vertexMap, character);
            int characterLength = pathLength(character);
            int *characterPath = (int*)malloc(characterLength * 2 * sizeof(int));
            characterPath = pathMap(start, finish, characterLength * 2 - 1, characterPath);
            matrix[i][j]->price = characterPrize;
            matrix[i][j]->length = characterLength;
            matrix[i][j]->path = characterPath;
        }
    }

    /*
     * pole array reprezentuje jednu permutáciu princezien
     */

    int *array = (int*)malloc((mapInfo->princess + 1) * sizeof(int));
    MATRIXNODE *best = NULL;
    for(int i = 1; i <= mapInfo->princess; i++){
        array[i-1] = i;
    }
    permutation(array, 0, mapInfo->princess-1, &best, matrix, mapInfo);

    int oldLength = length;
    length += best->length;
    int *finalPath =(int*) malloc(length * 2* sizeof(int));

    /*
     * spojenie cesty ku drakovi a cesty,ktorou
     * pozbieram všetky princezné
     */

    for (int k = 0; k < oldLength*2; ++k) {
        finalPath[k] = dragonPath[k];
    }
    for (int i = oldLength * 2, j = 0; i < length * 2; ++i, j++) {
        finalPath[i] = best->path[j];
    }
    freeMatrix(&matrix,mapInfo);

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
                f=fopen("D:\\Internet toto nie je\\Skola\\2 semester\\DSA  Datove struktury a algoritmy\\Velke zadania\\3-Tretie\\zadanie3\\testovanie.txt","r");
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