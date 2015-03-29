//
// Operation Mafghan Reconstruction by Harrison Ching
// hching@ucdavis.edu
//

#include <iostream>
#include <cstring>

using namespace std;

struct Road {
	int city1;
	int city2;
	int length;
	Road *pNext;
};

struct Region {
	Road *pRoadHead;
	Road *pRoadTail;
	int road_count;
	int smallest_city;
	Region *pNext;
};

void sort_region(Region *pRegion);
int compare_roads(const void *a, const void *b);
int compare_region(const void *a, const void *b);
void sort (void *base, size_t num, size_t size, int (*equ)(const void*,const void*));
void quick_sort(void *base, size_t size, int (*equ)(const void*,const void*), int first, int last);
int segment(void *base, size_t size, int (*equ)(const void*,const void*), int first, int last);

void sort_region(Region *pRegion)
{
	int i=0;
	Road *pRoad_arr, *pRoad;
	
	pRoad_arr = new Road [pRegion->road_count];
	for (i = 0, pRoad = pRegion->pRoadHead; pRoad != NULL; pRoad = pRoad->pNext, i++) {
		pRoad_arr[i].city1 = pRoad->city1;
		pRoad_arr[i].city2 = pRoad->city2;
		pRoad_arr[i].length = pRoad->length;
	}
	sort(pRoad_arr, pRegion->road_count, sizeof(Road), compare_roads);
	cout << "<region>" << endl;
	for (i = 0; i < pRegion->road_count; i++) {
    cout << "<road>";
    if (pRoad_arr[i].city1 < pRoad_arr[i].city2)
      cout << pRoad_arr[i].city1 << " " << pRoad_arr[i].city2 << " " << pRoad_arr[i].length << "</road>" << endl;
    else
      cout << pRoad_arr[i].city2 << " " << pRoad_arr[i].city1 << " " << pRoad_arr[i].length << "</road>" << endl;
  }
  cout << "</region>" << endl;
}

int compare_roads(const void *a, const void *b)
{
	Road *x, *y;
	int min_c, max_c, min_d, max_d;
	
	x = (Road*) a;
	y = (Road*) b;
	
	if (x->length == y->length) {
		min_c = (x->city1 < x->city2) ? x->city1 : x->city2;
		min_d = (y->city1 < y->city2) ? y->city1 : y->city2;
		if (min_c == min_d) {
			max_c = (x->city1 > x->city2) ? x->city1 : x->city2;
			max_d = (y->city1 > y->city2) ? y->city1 : y->city2;
			return(max_c - max_d);
		}
		else
			return(min_c - min_d);
	}
	else
		return(x->length - y->length);
	
}

int compare_region(const void *a, const void *b)
{
	Region *x, *y;
	
	x = (Region*) a;
	y = (Region*) b;

	if (x->road_count == y->road_count) {
		return(x->smallest_city - y->smallest_city);
	}
	else
		return(x->road_count - y->road_count);
}

void sort(void *base, size_t num, size_t size, int (*equ)(const void*,const void*))
{
	quick_sort(base, size, equ, 0, num-1);
}

void quick_sort(void *base, size_t size, int (*equ)(const void*,const void*), int first, int last)
{
	int pivot;
	if (first < last) {
		pivot = segment(base, size, equ, first, last);
		quick_sort(base, size, equ, first, pivot-1);
		quick_sort(base, size, equ, pivot+1, last);
	}
}

int segment(void *base, size_t size, int (*equ)(const void*,const void*), int first, int last)
{
	int point, x, y;
	char *pivot = new char [size];
	char *temp = new char [size];
	char *basep = (char*) base;
	
	point = (first+last)/2;
	memmove(temp, basep+first*size, size);
	memmove(basep+first*size, basep+point*size, size);
	memmove(basep+point*size, temp, size);
	memmove(pivot, basep+first*size, size);
	
	y = first;

	for (x = first+1; x <= last; x++) 
		if ((*equ)(basep+x*size, pivot) < 0) {
		y++;
		if (x == y)
			continue;
		memmove(temp, basep+y*size, size);
		memmove(basep+y*size, basep+x*size, size);
		memmove(basep+x*size, temp, size);
		}
	memmove(temp, basep+first*size, size);
	memmove(basep+first*size, basep+y*size, size);
	memmove(basep+y*size, temp, size);
	
	return(y);
}

int main()
{
	int i, j, city1, city2, length, old_road_count, city_count, region_count = 0;
	Road *pRoad;
	bool *pRoot, *p_dirty_road;
	Region *pRegion, *pRegionHead = NULL, *pRegionTail = NULL;
	Region *pRegion_arr;

	cin >> city_count;
	cin >> old_road_count;
	
	pRoot = new bool [city_count];			//set cities as roots
	pRoad = new Road [old_road_count];		//set roads as edges
	for (i=0; i < city_count; i++)
		pRoot[i] = false;
	p_dirty_road = new bool [old_road_count];
	for (i=0; i < old_road_count; i++)
		p_dirty_road[i] = false;
	
	for (i=0; i < old_road_count; i++) {	//input cities
		cin >> city1 >> city2 >> length;
		pRoad[i].city1 = city1;
		pRoad[i].city2 = city2;
		pRoad[i].length = length;
		pRoad[i].pNext = NULL;
	}

	sort(pRoad, old_road_count, sizeof(Road), compare_roads);	//sort roads

	while (1){
		for (j=0; j < city_count; j++) {	//start at city -> create region
			if (pRoot[j] == false) {
				pRoot[j] = true;
			break;
			}
		}
		if (j >= city_count)	//sort complete
			break;

		++region_count;			//create new regions
		pRegion = new Region;
		pRegion->pRoadHead = NULL;
		pRegion->pRoadTail = NULL;
		pRegion->road_count = 0;
		pRegion->smallest_city = j;
		pRegion->pNext = NULL;
		if (pRegionHead == NULL) {	//replace head if head = null
			pRegionHead = pRegion;
			pRegionTail = pRegion;
		}
		else {
			pRegionTail->pNext = pRegion;
			pRegionTail = pRegion;
		}
		
		for (i=0; i < old_road_count; i++) {	//if road is in the set, move region
			if (!p_dirty_road[i] && ((!pRoot[pRoad[i].city1] && pRoot[pRoad[i].city2]) || (pRoot[pRoad[i].city1] && !pRoot[pRoad[i].city2]))) {
				p_dirty_road[i] = true;
				pRoot[pRoad[i].city1] = true;
				pRoot[pRoad[i].city2] = true;
				if (pRegion->smallest_city > pRoad[i].city1)
					pRegion->smallest_city = pRoad[i].city1;
				if (pRegion->smallest_city > pRoad[i].city2)
					pRegion->smallest_city = pRoad[i].city2;
				++pRegion->road_count;
				if (pRegion->pRoadHead == NULL) {
					pRegion->pRoadHead = &pRoad[i];
					pRegion->pRoadTail = &pRoad[i];
				}
				else {
					pRegion->pRoadTail->pNext = &pRoad[i];
					pRegion->pRoadTail = &pRoad[i];
				}
				i = -1;
			}
		}
	}
	
	pRegion_arr = new Region [region_count];		//store region into arr
	for (j = 0, pRegion = pRegionHead; pRegion != NULL; pRegion = pRegion->pNext, j++) { //replace next region
		pRegion_arr[j].pRoadHead = pRegion->pRoadHead;
		pRegion_arr[j].pRoadTail = pRegion->pRoadTail;
		pRegion_arr[j].road_count = pRegion->road_count;
		pRegion_arr[j].smallest_city = pRegion->smallest_city;
	}
	
	sort(pRegion_arr, region_count, sizeof(Region), compare_region);	//sort regions
	
	cout << "<?xml version=\"1.5\"?>" << endl;
	cout << "<country>" << endl;
	for (int i = 0; i < region_count; i++) {							//print different cities
		sort_region(&pRegion_arr[i]);
	}
	cout << "</country>" << endl;
	
	return 0;
}
