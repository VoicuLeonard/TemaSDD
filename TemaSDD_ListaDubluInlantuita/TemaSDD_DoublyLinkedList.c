#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

#define LINESIZE 128
typedef struct Car Car;
typedef struct Node Node;
typedef struct DoublyLinkedList DLList;

struct Car {
	int year;
	char* manufacturer;
	float cc;
};

struct Node {
	Car car;
	Node* next;
	Node* prev;
};

struct DoublyLinkedList {
	Node* start;
	Node* end;
};

void insertAtBeginning(DLList* list, Car car) {
	Node* newNode = malloc(sizeof(Node));
	newNode->car = car;
	newNode->next = list->start;
	newNode->prev = NULL;
	if (list->start) {
		list->start->prev = newNode;
	}
	else {
		list->end = newNode;
	}
	list->start = newNode;
}

void insertAtEnd(DLList* list, Car car) {
	Node* newNode = malloc(sizeof(Node));
	newNode->car = car;
	newNode->prev = list->end;
	newNode->next = NULL;
	if (list->start) {
		list->end->next = newNode;
	}
	else {
		list->start = newNode;
	}
	list->end = newNode;
}

Car deleteCarFromBeginning(DLList* list) {
	if (list->start) {
		Node* toBeDeleted = list->start;
		Car carToBeDeleted = toBeDeleted->car;
		if (list->start->next) {
			list->start = list->start->next;
			list->start->prev = NULL;

			free(toBeDeleted);
			return carToBeDeleted;
		}
		else {
			free(toBeDeleted);
			list->start = NULL;
			list->end = NULL;

			return carToBeDeleted;
		}
	}
	else {
		printf("Lista este goala\n");
		Car err = { .year = 0,.manufacturer = "",.cc = 0 };
		return err;
	}
}

Car deleteCarFromEnd(DLList* list) {
	if (list->end) {
		Node* toBeDeleted = list->end;
		Car deletedCar = toBeDeleted->car;
		if (list->end->prev) {
			list->end = list->end->prev;
			list->end->next = NULL;

			free(toBeDeleted);
			return deletedCar;
		}
		else {
			free(toBeDeleted);
			list->start = NULL;
			list->end = NULL;
			return deletedCar;
		}
	}
	else {
		printf("Lista este goala\n");
		Car err = { .year = 0,.manufacturer = "",.cc = 0 };
		return err;
	}
}

void deleteCarByYear(DLList* list, int year) {
	if (list->start) {
		Node* current = list->start;

		while (current) {
			if (current->car.year == year) {
				Node* toBeDeleted = current;
        
				if (current == list->start) {
					list->start = current->next;
					if (list->start) {
						list->start->prev = NULL; 
					}
					else {
						list->end = NULL; 
					}
				}
			
				else if (current == list->end) {
					list->end = current->prev;
					if (list->end) {
						list->end->next = NULL;
					}
					else {
						list->start = NULL; 
					}
				}
			
				else {
					current->prev->next = current->next; 
					current->next->prev = current->prev; 
				}

				current = current->next; 
				free(toBeDeleted);
			}
			else {
				current = current->next;
			}
		}
	}
	else {
		printf("Lista este goala\n");
	}
}


void printCar(Car car) {
	printf("%s from %d has %5.2f\n", car.manufacturer, car.year, car.cc);

}

void parseBtoE(DLList list) {
	if (list.start) {
		while (list.start) {
			printCar(list.start->car);
			list.start = list.start->next;
		}
	}
	else {
		printf("list is empty\n");
	}
}

void parseEtoB(DLList list) {
	if (list.end) {
		while (list.end) {
			printCar(list.end->car);
			list.end = list.end->prev;
		}
	}
	else {
		printf("list is empty\n");
	}
}

Car readCarFromFile(FILE* f) {
	char buffer[LINESIZE];
	fgets(buffer, LINESIZE, f);
	Car car;

	char* carData = NULL;

	carData = strtok(buffer, ",");
	car.year = atoi(carData);

	carData = strtok(NULL, ",");
	car.manufacturer = malloc(sizeof(carData) * (strlen(carData) + 1));
	strcpy(car.manufacturer, carData);

	carData = strtok(NULL, ",");
	car.cc = atof(carData);

	return car;

}

void readCarsFromFile(DLList* list, int* noCars, const char* fileName) {
	FILE* f;
	f = fopen(fileName, "r");

	if (!f) {
		printf("Fisierul %s nu a putut fi deschis\n", fileName);
		return;
	}

	while (!feof(f)) {
		insertAtEnd(list, readCarFromFile(f));
		*noCars += 1;
	}

	fclose(f);
}




int main() {
	int noCars = 0;
	DLList list;
	list.start = NULL;
	list.end = NULL;

	readCarsFromFile(&list, &noCars, "cars.txt");
	parseBtoE(list);

	printf("---------------------------\n");
	printf("%d cars were read from file\n", noCars);

	printf("--------------------\n");
	parseEtoB(list);

	Car car1 = { 2022, "Tesla", 3.0 };
	insertAtEnd(&list, car1);
	printf("\n----------------------------------\n");
	printf("After inserting at the beginning:\n");
	parseBtoE(list);

	printf("---------------\n");
	Car deletedCar = deleteCarFromBeginning(&list);
	printf("Deleted Car:\n");
	printCar(deletedCar);

	printf("---------------------\n");
	parseBtoE(list);

	Car deletedCar1 = deleteCarFromEnd(&list);
	printf("Deleted Car:\n");
	printCar(deletedCar1);

	printf("-----------------\n");
	parseEtoB(list);

	deleteCarByYear(&list, 2019);
	printf("------------------\n");

	parseBtoE(list);

	return 0;

}
