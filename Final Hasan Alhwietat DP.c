#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#define SIZE 100

struct employees {
    int employeeId;
    char firstName[100];
    char lastName[100];
    char email[150];
    int departmentId;
    struct employees *next;
};

struct projects {
    int projectId;
    char projectName[100];
    char startDate[20];
    char endDate[20];
    struct projects *next;
};

struct departments {
    int departmentId;
    char departmentName[100];
    struct departments *next;
};

struct employeeProjects {
    int employeeId;
    int projectId;
    int hoursWorked;
    char status[20];
    struct employeeProjects *next;
};

struct employees *empFirst = NULL;
struct employeeProjects *empProjFirst = NULL;
struct projects *projFirst = NULL;
struct departments *deptFirst = NULL;

#define SIZE 100
int hashArray[SIZE] = {0};

int hashFunction(int value) {
    return (value / 5 + 1) % SIZE;
}

void inserthashArray(int value) {
    int index = hashFunction(value);
    int counter = 0;
    while (hashArray[index] != 0) {
        counter++;
        index = (index + counter * counter) % SIZE;
    }
    hashArray[index] = value;
}

void display_function() {
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", hashArray[i]);
    }
    printf("\n");
}    

int searchhashArray(int value) {
    int index = hashFunction(value);
    int counter = 0;
    while (hashArray[index] != value) {
        if (hashArray[index] == 0) {
            return -1;
        }
        counter++;
        index = (index + counter * counter) % SIZE;
    }
    return index;
}

void deletehashArray(int value) {
    int index = searchhashArray(value);
    if (index != -1) {
        hashArray[index] = 0;
    }
}

int checkEmail(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    return at != NULL && dot != NULL && at < dot;
}

void writeEmployeeFile(){
    FILE *file = fopen("employees.txt", "w");
    struct employees *pointerEmp = empFirst;
    while (pointerEmp) {
        fprintf(file, "%d %s %s %s %d\n", pointerEmp->employeeId, pointerEmp->firstName, pointerEmp->lastName, pointerEmp->email, pointerEmp->departmentId);
        pointerEmp = pointerEmp->next;
    }
    fclose(file);
}

void readEmployeeFile() {
    struct employees *newEmp;
    struct employees *lastEmp = NULL;
    FILE *file = fopen("employees.txt", "r");
    if (file == NULL) {
        return;
    }
    while (!feof(file)) {
        newEmp = (struct employees *)malloc(sizeof(struct employees));
        if (newEmp != NULL && fscanf(file, "%d %s %s %s %d", &newEmp->employeeId, newEmp->firstName, newEmp->lastName, newEmp->email, &newEmp->departmentId) == 5) {
            newEmp->next = NULL;
            if (empFirst == NULL) {
                empFirst = newEmp; 
            } else {
                lastEmp->next = newEmp; 
            }
            lastEmp = newEmp; 
        } else {
            free(newEmp);
        }
    }
    fclose(file);
}

void insertEmployee() {
    char input[20];
    struct employees *pointerEmp = empFirst;
    
    struct employees *newEmp = (struct employees *)malloc(sizeof(struct employees));
    if (newEmp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Please enter employee id: ");
    if (scanf("%s", input) != 1) {
        printf("Employee ID must integer\n");
        free(newEmp);
        return;
    }
    newEmp->employeeId = atoi(input);

    while (pointerEmp != NULL) {
        if (pointerEmp->employeeId == newEmp->employeeId) {
            printf("Employee ID already exists\n");
            free(newEmp);
            return;
        }
        pointerEmp = pointerEmp->next;
    }

    printf("Please enter first name: ");
    scanf("%s", newEmp->firstName);
    if (strlen(newEmp->firstName) == 0) {
        printf("First name not NULL\n");
        free(newEmp);
        return;
    }

    printf("Please enter last name: ");
    scanf("%s", newEmp->lastName);
    if (strlen(newEmp->lastName) ==0) {
        printf("Last name not NULL\n");
        free(newEmp);
        return;
    }

    printf("Please enter email: ");
    scanf("%s", newEmp->email);
    if (!checkEmail(newEmp->email)) {
        printf("email not correct\n");
        free(newEmp);
        return;
    }

    printf("Please enter department id: ");
    if (scanf("%s", input) != 1) {
        printf("Department ID must integer\n");
        free(newEmp);
        return;
    }
    newEmp->departmentId = atoi(input);

    newEmp->next = NULL;
    if (empFirst == NULL) {
        empFirst = newEmp;
    } else {
        struct employees *last = empFirst;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newEmp;
    }

    inserthashArray(newEmp->employeeId);
    writeEmployeeFile();
    printf("The Employee is insert successfully\n");
}

void writeEMployeeProjectsFile(){
    FILE *file = fopen("employee_projects.txt", "w");
    struct employeeProjects *pointerEmpProj = empProjFirst;
    while (pointerEmpProj) {
        fprintf(file, "%d %d %d %s\n", pointerEmpProj->employeeId, pointerEmpProj->projectId, pointerEmpProj->hoursWorked, pointerEmpProj->status);
        pointerEmpProj = pointerEmpProj->next;
    }
    fclose(file);
}

void readEmployeeProjectsFile() {
    struct employeeProjects *newEmpProj;
    struct employeeProjects *lastEmpProj = NULL; 
    FILE *file = fopen("employee_projects.txt", "r");
    if (file == NULL) {
        return;
    }
    while (!feof(file)) {
        newEmpProj = (struct employeeProjects *)malloc(sizeof(struct employeeProjects));
        if (newEmpProj != NULL && fscanf(file, "%d %d %d %s", &newEmpProj->employeeId, &newEmpProj->projectId, &newEmpProj->hoursWorked, newEmpProj->status) == 4) {
            newEmpProj->next = NULL;
            if (empProjFirst == NULL) {
                empProjFirst = newEmpProj; 
            } else {
                lastEmpProj->next = newEmpProj; 
            }
            lastEmpProj = newEmpProj; 
            } else {
            free(newEmpProj);
        }
    }
    fclose(file);
}

void insertEmployeeProjects() {
    char input[20];
    struct employeeProjects *newEmpProj = (struct employeeProjects *)malloc(sizeof(struct employeeProjects));
    if (newEmpProj == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    printf("Please enter employee id: ");
    if (scanf("%s", input) != 1) {
        printf("Employee ID must integer\n");
        free(newEmpProj);
        return;
    }
    newEmpProj->employeeId = atoi(input);

    printf("Please enter project id: ");
    if (scanf("%s", input) != 1) {
        printf("Project ID must integer\n");
        free(newEmpProj);
        return;
    }
    newEmpProj->projectId = atoi(input);

    struct employeeProjects *pointerEmpProj = empProjFirst;
    while (pointerEmpProj != NULL) {
        if (pointerEmpProj->employeeId == newEmpProj->employeeId && pointerEmpProj->projectId == newEmpProj->projectId) {
            printf("Employee ID and Project ID already exist\n");
            free(newEmpProj);
            return;
        }
        pointerEmpProj = pointerEmpProj->next;
    }

    printf("Please enter hours worked: ");
    if (scanf("%s", input) != 1) {
        printf("The hours worked must integer\n");
        free(newEmpProj);
        return;
    }
    newEmpProj->hoursWorked = atoi(input);
    
    printf("Please enter status: ");
    scanf("%s", newEmpProj->status);
    if (strlen(newEmpProj->status) == 0) {
        printf("Status not empty\n");
        free(newEmpProj);
        return;
    }

    newEmpProj->next = NULL;
    if (empProjFirst == NULL) {
        empProjFirst = newEmpProj;
    } else {
        struct employeeProjects *last = empProjFirst;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = newEmpProj;
    }
    writeEmployeeFile();
    printf("The Employee Project is insert successfully\n");
}

void updateEmployeeByID(int empID) {
    int updated = 0, newEmpID;
    struct employees *pointerEmp = empFirst;
    while (pointerEmp) {
        if (pointerEmp->employeeId == empID) {
            char newFirstName[100], newLastName[100], newEmail[150];
            printf("Please enter new first name: ");
            scanf("%s", newFirstName);
            if (strlen(newFirstName) == 0) {
                printf("First name empty\n");
                return;
            }
            printf("Please enter new last name: ");
            scanf("%s", newLastName);
            if (strlen(newLastName) == 0) {
                printf("Last name empty\n");
                return;
            }
            printf("Please enter new email: ");
            scanf("%s", newEmail);
            if (!checkEmail(newEmail)) {
                printf("email not correct\n");
                return;
            }
            printf("Please enter new department id: ");
            scanf("%d", &pointerEmp->departmentId);
            deletehashArray(empID);
            inserthashArray(empID);
            strcpy(pointerEmp->firstName, newFirstName);
            strcpy(pointerEmp->lastName, newLastName);
            strcpy(pointerEmp->email, newEmail);
            writeEmployeeFile();
            updated = 1;
            break;
        } 
        pointerEmp = pointerEmp->next;
    }
    if (updated) {
        printf("Employee information updated successfully\n");
    } else {
        printf("Employee not found\n");
    }
}

void updateEmployeeProjectByID(int empID, int projID) {
    int updated = 0;
    struct employeeProjects *pointerEmpProj = empProjFirst;
    
    while (pointerEmpProj) {
        if (pointerEmpProj->employeeId == empID && pointerEmpProj->projectId == projID) {
            int newHoursWorked;
            char newStatus[20];
            printf("Please enter new hours worked: ");
            if (scanf("%d", &newHoursWorked) != 1) {
                printf("Hours worked must integer\n");
                return;
            }
            if (newHoursWorked < 0) {
                printf("Hours worked must not negative\n");
                return;
            }
            
            printf("Please enter new status: ");
            scanf("%s", newStatus);
            if (strlen(newStatus) == 0) {
                printf("Status not empty\n");
                return;
            }
            
            strcpy(pointerEmpProj->status, newStatus);
            pointerEmpProj->hoursWorked = newHoursWorked;
            writeEMployeeProjectsFile();
            updated = 1;
            break;
        } 
        pointerEmpProj = pointerEmpProj->next;
    }
    if (updated) {
        printf("Employee project information updated successfully.\n");
    } else {
        printf("Employee project not found.\n");
    }
}

void deleteEmployeeProjectForgenKey(int empID) {
    int deleted = 0;
    struct employeeProjects *pointerEmpProjForgen = empProjFirst, *prev = NULL;
    while (pointerEmpProjForgen) {
        if (pointerEmpProjForgen->employeeId == empID) {
            if (prev) {
                prev->next = pointerEmpProjForgen->next;
            } else {
                empProjFirst = pointerEmpProjForgen->next;
            }
            free(pointerEmpProjForgen);
            pointerEmpProjForgen = (prev) ? prev->next : empProjFirst;
            deleted = 1;
            break;
        } else {
            prev = pointerEmpProjForgen;
            pointerEmpProjForgen = pointerEmpProjForgen->next;
        }
    }
    if (deleted) {
        printf("Employee projects deleted successfully\n");
        writeEMployeeProjectsFile();
    } else {
        printf("Employee projects not found\n");
    }
}

void deleteEmployeeByID(int empID) {
    int deleted = 0;
    struct employees *pointerEmp = empFirst, *prev = NULL;
    while (pointerEmp) {
        if (pointerEmp->employeeId == empID) {
            if (prev) {
                prev->next = pointerEmp->next;
            } else {
                empFirst = pointerEmp->next;
            }
            free(pointerEmp);
            deleted = 1;
            break;
        } 
        prev = pointerEmp;
        pointerEmp = pointerEmp->next;
    }
    if (deleted) {
        printf("Employee deleted successfully\n");
        deleteEmployeeProjectForgenKey(empID);
        writeEmployeeFile();
        deletehashArray(empID);
    } else {
        printf("Employee not found\n");
    }
}

void deleteEmployeeProjectByID(int empID, int projID){
    int deleted = 0;
    struct employeeProjects *pointerEmpProj = empProjFirst;
    struct employeeProjects *prev = NULL;
    while (pointerEmpProj != NULL) {
        if (pointerEmpProj->employeeId == empID && pointerEmpProj->projectId == projID) {
            if (prev == NULL) {
                empProjFirst = pointerEmpProj->next;
            } else {
                prev->next = pointerEmpProj->next;
            }
            free(pointerEmpProj);
            deleted = 1;
            break;
        }
        prev = pointerEmpProj;
        pointerEmpProj = pointerEmpProj->next;
    }

    if (deleted) {
        printf("Employee project deleted successfully\n");
        writeEMployeeProjectsFile();
    } else {
        printf("Employee project not found\n");
    }
}

void *displayEmployees(void *arg) {
    int found = 0;
    struct employees *pointerEmp = empFirst;
    while (pointerEmp != NULL) {
        printf("ID: %d, ", pointerEmp->employeeId);
        printf("First Name: %s, ", pointerEmp->firstName);
        printf("Last Name: %s, ", pointerEmp->lastName);
        printf("Email: %s, ", pointerEmp->email);
        printf("Department ID: %d\n", pointerEmp->departmentId);
        found = 1;
        pointerEmp = pointerEmp->next;
    }
    if (!found) {
        printf("Not found employees in database\n");
    }
    return NULL;
}

void displayEmployeeProjects() {
    int found = 0;
    struct employeeProjects *pointerEmpProj = empProjFirst;
    while (pointerEmpProj != NULL) {
        printf("Employee ID: %d, ", pointerEmpProj->employeeId);
        printf("Project ID: %d, ", pointerEmpProj->projectId);
        printf("Hours Worked: %d, ", pointerEmpProj->hoursWorked);
        printf("Status: %s\n", pointerEmpProj->status);
        pointerEmpProj = pointerEmpProj->next;
        found = 1;
    }
    if (!found) {
        printf("Not found employee project in database\n");
    }
}

void readDepartment() {
    struct departments *newDept;
    struct departments *lastDept = NULL; 
    FILE *file = fopen("departments.txt", "r");
    if (file == NULL) {
        return;
    }
    while (!feof(file)) {
        newDept = (struct departments *)malloc(sizeof(struct departments));
        if (newDept != NULL && fscanf(file, "%d %s", &newDept->departmentId, newDept->departmentName) == 2) {
            newDept->next = NULL;



            if (deptFirst == NULL) {
                deptFirst = newDept;
            } else {
                lastDept->next = newDept; 
            }
            lastDept = newDept; 
        } else {
            free(newDept);
        }
    }
    fclose(file);
}

void displayDepartments() {
    int found = 0;
    struct departments *pointerDepart = deptFirst;
    while (pointerDepart != NULL) {
        printf("Department ID: %d, ", pointerDepart->departmentId);
        printf("Department Name: %s\n", pointerDepart->departmentName);
        pointerDepart = pointerDepart->next;
        found = 1;
    }
    if (!found) {
        printf("Not found employees in database\n");
    }
}

void readProject() {
    struct projects *newProj;
    struct projects *lastProj = NULL; 
    FILE *file = fopen("projects.txt", "r");
    if (file == NULL) {
        return;
    }

    while (!feof(file)) {
        newProj = (struct projects *)malloc(sizeof(struct projects));
        if (newProj != NULL && fscanf(file, "%d %s %s %s", &newProj->projectId, newProj->projectName, newProj->startDate, newProj->endDate) == 4) {
            newProj->next = NULL;
            if (projFirst == NULL) {
                projFirst = newProj; 
            } else {
                lastProj->next = newProj;
            }
            lastProj = newProj; 
        } else {
            free(newProj);
        }
    }
    fclose(file);
}

void displayProjects() {
    int found = 0;
    struct projects *pointerProj = projFirst;
    while (pointerProj != NULL) {
        printf("Project ID: %d, ", pointerProj->projectId);
        printf("Project Name: %s, ", pointerProj->projectName);
        printf("Start Date: %s, ", pointerProj->startDate);
        printf("End Date: %s\n", pointerProj->endDate);
        pointerProj = pointerProj->next;
        found = 1;
    }
    if (!found) {
        printf("Not found employees in database\n");
    }
}

void searchEmployeeByID(int empID) {
    int found = 0;
    struct employees *pointerEmp = empFirst;
    while (pointerEmp != NULL) {
        if (pointerEmp->employeeId == empID) {
            printf("ID: %d, ", pointerEmp->employeeId);
            printf("First Name: %s, ", pointerEmp->firstName);
            printf("Last Name: %s, ", pointerEmp->lastName);
            printf("Email: %s, ", pointerEmp->email);
            printf("Department ID: %d\n", pointerEmp->departmentId);
            found = 1;
            break;
        }
        pointerEmp = pointerEmp->next;
    }
    
    if (!found) {
        printf("Employee not found\n");
    }
}

void searchEmployeeProjectByID(int empID, int projID) {
    int found = 0;
    struct employeeProjects *pointerEmpProj = empProjFirst;
    while (pointerEmpProj != NULL) {
        if (pointerEmpProj->employeeId == empID && pointerEmpProj->projectId == projID) {
            printf("Employee ID: %d, ", pointerEmpProj->employeeId);
            printf("Project ID: %d, ", pointerEmpProj->projectId);
            printf("Hours Worked: %d, ", pointerEmpProj->hoursWorked);
            printf("Status: %s\n", pointerEmpProj->status);
            found = 1;
            break;
        }
        pointerEmpProj = pointerEmpProj->next;
    }
    if (!found) {
        printf("Employee project not found\n");
    }
}

void searchDepartmentByID(int depID) {
    int found = 0;
    struct departments *pointerDepart = deptFirst;
    while (pointerDepart != NULL) {
        if (pointerDepart->departmentId == depID) {
            printf("Department ID: %d, ", pointerDepart->departmentId);
            printf("Department Name: %s\n", pointerDepart->departmentName);
            found = 1;
            break;
        }
        pointerDepart = pointerDepart->next;
    }
    if (!found) {
        printf("Department not found\n");
    }
}

void searchProjectByID(int projID) {
    int found = 0;
    struct projects *pointerProj = projFirst;
    while (pointerProj != NULL) {
        if (pointerProj->projectId == projID) {
            printf("Project ID: %d, ", pointerProj->projectId);
            printf("Project Name: %s, ", pointerProj->projectName);
            printf("Start Date: %s, ", pointerProj->startDate);
            printf("End Date: %s\n", pointerProj->endDate);
            found = 1;
            break;
        }
        pointerProj = pointerProj->next;
    }
    if (!found) {
        printf("Project not found\n");
    }
}

void searchEmployeeByDepartmentID(int deptID) {
    int found = 0;
    struct employees *pointerEmpDep = empFirst;
    while (pointerEmpDep != NULL) {
        if (pointerEmpDep->departmentId == deptID) {
            printf("Employee ID: %d, ", pointerEmpDep->employeeId);
            printf("First Name: %s, ", pointerEmpDep->firstName);
            printf("Last Name: %s, ", pointerEmpDep->lastName);
            printf("Email: %s, ", pointerEmpDep->email);
            printf("Department ID: %d\n", pointerEmpDep->departmentId);
            found = 1;
        }
        pointerEmpDep = pointerEmpDep->next;
    }
    if (!found) {
        printf("No employees found in Department ID %d\n", deptID);
    }
}

void searchEmployeeProjectsJoinProjectsByEmpId(int empID) {
    int found = 0;
    struct employeeProjects *pointerEmpProj = empProjFirst;
    struct projects *pointerProj = projFirst;

    while (pointerEmpProj != NULL) {
        if (pointerEmpProj->employeeId == empID) {
            while (pointerProj != NULL) {
                if (pointerProj->projectId == pointerEmpProj->projectId) {
                    printf("Project Name: %s, ", pointerProj->projectName);
                    printf("Hours Worked: %d, ", pointerEmpProj->hoursWorked);
                    printf("Status: %s, ", pointerEmpProj->status);
                    printf("Start Date: %s, ", pointerProj->startDate);
                    printf("End Date: %s\n", pointerProj->endDate);
                    found = 1;
                }
                pointerProj = pointerProj->next;
            }
        }
        pointerEmpProj = pointerEmpProj->next;
    }
    if (!found) {
        printf("No projects found for Employee ID %d\n", empID);
    }
}

void searchEmployeeProjectsJoinEmployeesByProjId(int projID) {
    int found = 0;
    struct employeeProjects *pointerEmpProj = empProjFirst;

    while (pointerEmpProj != NULL) {
        if (pointerEmpProj->projectId == projID) {
            struct employees *empPtr = empFirst;
            while (empPtr != NULL) {
                if (empPtr->employeeId == pointerEmpProj->employeeId) {
                    struct projects *pointerProj = projFirst;
                    while (pointerProj != NULL) {
                        if (pointerProj->projectId == projID) {
                            printf("First Name: %s, ", empPtr->firstName);
                            printf("Last Name: %s, ", empPtr->lastName);
                            printf("Project Name: %s, ", pointerProj->projectName);
                            printf("Hours Worked: %d\n", pointerEmpProj->hoursWorked);
                            found = 1;
                            break;
                        }
                        pointerProj = pointerProj->next;
                    }
                    break;
                }
                empPtr = empPtr->next;
            }
        }
        pointerEmpProj = pointerEmpProj->next;
    }
    if (!found) {
        printf("Projects not found for Project ID\n");
    }
}

void mainMenuManagerSearch(){
    int choice, empID, projID, depID;
    do {
        printf("\nWelcome to the Manager main menu Search:\n");
        printf("1. Search Employee By ID\n");
        printf("2. Search Employee Project By ID\n");
        printf("3. Search Department By ID\n");
        printf("4. Search Project By ID\n");
        printf("5. Search Employee By Department ID\n");
        printf("6. Search Employee Project Join Project By Employee ID\n");
        printf("7. Search Employee Project Join Employee By Project ID\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("choice must integer\n");
            return;
        }
        switch (choice) {
            case 1: 
                printf("Enter the Employee ID: ");
                if (scanf("%d", &empID) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                searchEmployeeByID(empID); break;
            case 2: 
                printf("Enter the Employee ID: ");
                if (scanf("%d", &empID) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                printf("Enter the Project ID: ");
                if (scanf("%d", &projID) != 1) {
                    printf("Project ID must integer\n");
                    return;
                }
                searchEmployeeProjectByID(empID, projID); break;
            case 3: 
                printf("Enter the Department ID: ");
                if (scanf("%d", &depID) != 1) {
                    printf("Department ID must integer\n");
                    return;
                }
                searchDepartmentByID(depID); break;
            case 4: 
                printf("Enter the Project ID: ");
                if (scanf("%d", &projID) != 1) {
                    printf("Project ID must integer\n");
                    return;
                }
                searchProjectByID(projID); break;
            case 5: 
                printf("Enter the Department ID: ");
                if (scanf("%d", &depID) != 1) {
                    printf("Department ID must integer\n");
                    return;
                }
                searchEmployeeByDepartmentID(depID); break;
            case 6: 
                printf("Enter the Employee ID: ");
                if (scanf("%d", &empID) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                searchEmployeeProjectsJoinProjectsByEmpId(empID); break;
            case 7: 
                printf("Enter the Project ID: ");
                if (scanf("%d", &projID) != 1) {
                    printf("Project ID must integer\n");
                    return;
                }
                searchEmployeeProjectsJoinEmployeesByProjId(projID); break;
            case 8: printf("Exit the main menu\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);
}

void mainMenuManagerDisplay(){
    int choice;
    pthread_t tid[0];
    do {
        printf("\nWelcome to the Manager main menu Display:\n");
        printf("1. Display Employee\n");
        printf("2. Display Employee Project\n");
        printf("3. Display Department\n");
        printf("4. Display Project\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("choice must integer\n");
            return;
        }
        switch (choice) {
            case 1: 
                pthread_create(&tid[0], NULL, displayEmployees, NULL);
                pthread_join(tid[0], NULL);
                break;
            case 2: displayEmployeeProjects(); break;
            case 3: displayDepartments(); break;
            case 4: displayProjects(); break;
            case 5: printf("Exit the main menu\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);
}

void mainMenuManagerDelete(){
    int choice, empID, projID;
    do {
        printf("\nWelcome to the Manager main menu Delete:\n");
        printf("1. Delete Employee\n");
        printf("2. Delete Employee Project\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("choice must integer\n");
            return;
        }
        switch (choice) {
            case 1: 
                printf("Enter the Employee ID to delete: ");
                if (scanf("%d", &empID) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                deleteEmployeeByID(empID); break;
            case 2: 
                printf("Enter the Employee Project to delete first ID Employee: ");
                if (scanf("%d", &empID) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                printf("Enter the Employee Project to delete first ID Project: ");
                if (scanf("%d", &projID) != 1) {
                    printf("Project ID must integer\n");
                    return;
                }
                deleteEmployeeProjectByID(empID, projID); break;
            case 3: printf("Exit the main menu\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 3);
}

void mainMenuManager() {
    int choice, empID, projID;
    do {
        printf("\nWelcome to the Manager main menu:\n");
        printf("1. Insert Employee Project\n");
        printf("2. Update information Project\n");
        printf("3. Delete MENU\n");
        printf("4. Display MENU\n");
        printf("5. Search MENU\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("choice must integer\n");
            return;
        }
        switch (choice) {
            case 1: insertEmployeeProjects(); break;
            case 2: 
                printf("Enter the Employee Project to update first Employee ID: ");
                if (scanf("%d", &empID) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                printf("Enter the Employee Project to update second Project ID: ");
                if (scanf("%d", &projID) != 1) {
                    printf("Project ID must integer\n");
                    return;
                }
                updateEmployeeProjectByID(empID, projID); break;
            case 3: mainMenuManagerDelete(); break;
            case 4: mainMenuManagerDisplay(); break;
            case 5: mainMenuManagerSearch(); break;
            case 6: printf("Exit the main menu\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);
}

void mainMenuEmployeeSearch(){
    int choice, id;
    do {
        printf("\nWelcome to the Employee main menu Search:\n");
        printf("1. Search Employee By ID\n");
        printf("2. Search Employee By Department ID \n");
        printf("3. Search Employee Project Join Project By Employee ID\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("choice must integer\n");
            return;
        }
        switch (choice) {
            case 1:
                printf("Enter the Employee ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                searchEmployeeByID(id);
                break;
            case 2: 
                printf("Enter the Department ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Department ID must integer\n");
                    return;
                }
                searchEmployeeByDepartmentID(id);
                break;
            case 3: 
                printf("Enter the Employee ID: ");
                if (scanf("%d", &id) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                searchEmployeeProjectsJoinProjectsByEmpId(id);
                break;
            case 4: printf("Exit the main menu\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);
}

void mainMenuEmployee() {
    int choice, empID;
    do {
        printf("\nWelcome to the Employee main menu:\n");
        printf("1. Insert Employee\n");
        printf("2. Update Informations Employee\n");
        printf("3. Search MENU\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("choice must integer\n");
            return;
        }
        switch (choice) {
            case 1: insertEmployee(); break;
            case 2: 
                printf("Enter the Employee ID to update: ");
                if (scanf("%d", &empID) != 1) {
                    printf("Employee ID must integer\n");
                    return;
                }
                updateEmployeeByID(empID);break;
            case 3: mainMenuEmployeeSearch(); break;
            case 4: printf("Exit the main menu\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);
}

void insertData(){
    FILE *empFile = fopen("employees.txt", "w");
    fprintf(empFile, "1 Hasan Alhwietat Hasan@gmail.com 1\n");
    fprintf(empFile, "2 Lina Hammad Lina@gmail.com 2\n");
    fprintf(empFile, "3 Ahmad Nabel Ahmad@gmail.com 3\n");
    fprintf(empFile, "4 Ali bob Ali@gmail.com 4\n");
    fclose(empFile);

    FILE *empProjFile = fopen("employee_projects.txt", "w");
    fprintf(empProjFile, "1 1 55 Finshed\n");
    fprintf(empProjFile, "2 2 50 Finshed\n");
    fprintf(empProjFile, "3 1 13 NotFinshed\n");
    fprintf(empProjFile, "4 3 30 NotFinshed\n");
    fclose(empProjFile);
    
    FILE *projFile = fopen("projects.txt", "w");
    fprintf(projFile, "1 ProjectPython 2024-03-01 2024-08-31\n");
    fprintf(projFile, "2 ProjectStars 2024-01-01 2024-06-30\n");
    fprintf(projFile, "3 ProjectC 2024-03-01 2024-08-31\n");
    fclose(projFile);

    FILE *deptFile = fopen("departments.txt", "w");
    fprintf(deptFile, "1 Sales\n");
    fprintf(deptFile, "2 Marketing\n");
    fprintf(deptFile, "3 Engineering\n");
    fprintf(deptFile, "4 HR\n");
    fclose(deptFile);
}

int main() {
    int choice;
    insertData();
    readEmployeeFile();
    readDepartment();
    readEmployeeProjectsFile();
    readProject();
    do {
        printf("\nWelcome to the Tech Solutions database:\n");
        printf("1. Manager\n");
        printf("2. Employee\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("choice must integer\n");
            return 0;
        }
        switch (choice) {
            case 1: mainMenuManager(); break;
            case 2: mainMenuEmployee(); break;
            case 3: printf("Exit the program\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 3);
    return 0;
}