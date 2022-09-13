/*
credential file is a file that holds important information about users,
including their usernames and hashed passwords. The purpose of this program is
to add a new user to the credential file. Since only the user admin has the
proper permissions to manage users (e.g., adding a new user), the program will
first retrieve all username and passwords from credential file, stores them in
an array of struct user, iterates through the array to find the password (in the
hashed form) for user admin, and then asks the user to enter the admin password.
Only when the passwords match, the user is asked to provide the information
regarding the new user. The program will then add the user to the array of
struct user, and then write the updated list of users to credential file.
Consider the struct user used in Part I. In this part, we improve the security
of our login program by storing usernames and passwords in credential file.
credential file has the following format (separated by â€˜\tâ€™): firstname
lastname username password admin Note that the stored password in the file is
H(password), the hash of the original password. Since you are using 1 the lab
machines for this part, you should copy over labFileIO.c and credential file to
a lab machine first . Open credential file to see its contents.
*/


#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_SIZE 100
#define FILENAME "credential_file"

struct user {
  char username[50];
  char password[256];
  char firstname[50];
  char lastname[50];
  int admin;
};

// take off the const
char *cs336Hash(const char *password) {
   return crypt(password, "00");
}

struct user *createUsers(int *count) {
  // fopen?
  FILE *fpt = fopen(FILENAME, "r");

  // check to make sure that this file was opened
  if (fpt == NULL) {
    printf("Error: can't open file: %s\n", FILENAME);
    exit(1);
  }

  char *line = (char *)malloc(sizeof(char) * MAX_LINE_SIZE);
  // int length = sizeof(line);

  *count = 0;
  // while not at the end of file
  // fgets returns null when the EOF (End Of File) token is read
  while (fgets(line, MAX_LINE_SIZE, fpt) != NULL) {
    (*count)++;
  }

  // we know how many users so need to allocate the space
  struct user *users = (struct user *)malloc(sizeof(struct user) * (*count));
  fclose(fpt); // close the file

  return users;
}
// Vahab	Pournaghshband	vahab	00teY4w30eN8.	0
// first  last            user     pass          admin
void populateUsers(void *users) {

  FILE *fpt = fopen(FILENAME, "r");

  // check to make sure that this file was opened
  if (fpt == NULL) {
    printf("Error: can't open file: %s\n", FILENAME);
    exit(1);
  }

  char *line = (char *)malloc(sizeof(char) * MAX_LINE_SIZE);

  struct user *newUsers = (struct user *)users;

  int userCo = 0;
  // while not at the end of file
  // fgets returns null when the EOF (End Of File) token is read
  while (fgets(line, MAX_LINE_SIZE, fpt) != NULL) {

    // can use 'sscanf' to read through an existing "string" (char array)
    // already have the full line, so read up to the first space and
    // save that as the first name

    // the users pointer that is passed in is void so need to cast each
    // element as a user structure so the compiler knows that it has
    // firstname,lastname, etc....
    sscanf(line, "%s %s %s %s %d", (newUsers + userCo)->firstname,
           (newUsers + userCo)->lastname, (newUsers + userCo)->username,
           (newUsers + userCo)->password, &(newUsers + userCo)->admin);

    userCo++;
  }

  fclose(fpt);

}

int checkAdminPassword(char *password, struct user *users, int count) {
  for (int i = 0; i < count; ++i) {
    if (strcmp((users + i)->username, "admin") == 0) {
      // list of users that are being checked
      // are currently accessing the user struct for the admin
      struct user userAdmin = *(users + i);
      // the password is plain text need to hash it
      if (strcmp(cs336Hash(password), userAdmin.password) == 0) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  return 0;
}

struct user *addUser(struct user *users, int *count, char *username,
                     char *password, char *firstname, char *lastname,
                     int admin) {

  // reallocate space for the new user
  users = (struct user *)realloc(users, sizeof(struct user) * (*count + 1));
  if (users == NULL) {
    printf("Error: reallocation when adding user failed!\n");
    exit(1);
  }

  // wait to update the original count variable until after the
  // users array was successfully reallocated
  strcpy((users + *count)->username, username);
  strcpy((users + *count)->password, password);
  strcpy((users + *count)->firstname, firstname);
  strcpy((users + *count)->lastname, lastname);
  (users + *count)->admin = admin;

  *count += 1;
  return users;
}

void saveUsers(struct user *users, int count) {

  FILE *fpt = fopen(FILENAME, "w");

  // check to make sure that this file was opened
  if (fpt == NULL) {
    printf("Error: can't open file for writing in SaveUsers: %s\n", FILENAME);
    exit(1);
  }

  // Vahab	Pournaghshband	vahab	00teY4w30eN8.	0
  // first  last            user     pass          admin
for (int i = 0; i < count; i++) {
    fprintf(fpt, "%s %s %s %s %d\n", (users + i)->firstname,
           (users + i)->lastname, (users + i)->username, (users + i)->password,
            (users + i)->admin);
 
  }

  fclose(fpt);
}

int main(void) {
  int user_count = 0;
  struct user *users = createUsers(&user_count);

  if (users == NULL) {
    return EXIT_FAILURE;
  }
  populateUsers(users);

  printf("Enter admin password to add new users:");
  char entered_admin_password[50];
  scanf("%s", entered_admin_password); 
  if (checkAdminPassword(entered_admin_password, users, user_count)) {
    struct user new_user;
     printf("Enter username:");
     scanf("%s", new_user.username);
     printf("Enter first name:");
     scanf("%s", new_user.firstname);
     printf("Enter last name:");
     scanf("%s", new_user.lastname);
     printf("Enter password:");
     scanf("%s", new_user.password);
     printf("Enter admin level:");
     scanf("%d", &(new_user.admin));
    users = addUser(users, &user_count, new_user.username, new_user.password,
                    new_user.firstname, new_user.lastname, new_user.admin);
    if (users == NULL) {
      return EXIT_FAILURE;
    }
  }
  saveUsers(users, user_count);
  free(users);
  return EXIT_SUCCESS;
}

/*
1. Complete createUsers, which reads the file credential file, and counts the
number of lines in this file . The function, then, dynamically creates an array
of struct user based on the number of lines. This function returns the pointer
to the array, and also updates count which is the number of users.

2. Complete populateUsers, which reads the file credential file line-by-line,
gets the firstname, lastname, username, password and admin from each line to
populate users array


3. After calling populateUsers, the program asks the user to input the password
for username â€œadminâ€�. Then it checks if the entered password for admin is
correct by calling checkAdminPassword. Part of this function has been
implemented, complete the statement in if(). Keep in mind that the entered
password is in plaintext whereas the password in struct user is in the hashsed
form. The password for user admin is â€œs#1Pa5â€�.


4. Complete addUser, which by calling realloc, allocates enough memory space to
include the information about the new user. The function then updates count and
returns the pointer of the new array. Keep in mind that password stored in
struct user must be in the hashed form.

int * ptr = (123 john lane) but this only holds 5 houses
//to add a 6th house
int * newPtr = allocate space for 6 houses (150 bill lane)
copy all the old houses to the new address
free all the old houses at 123 john lane
ptr = newPtr;

WITH REALLOC

ptr = realloc(size )

5. Complete saveUsers, which write count users in users to the file credential
file using the same format as specified.
*/
