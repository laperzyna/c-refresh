// In this lab, you are to implement a login program, that stores and manages users credentials.
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// int * ptr; //assume we allocate and fill it with 2,4,6
//  ptr[0] - this says drive to the address of the pointer and move 0 houses to the right, THEN grab the value
//  *(ptr + 2) - this says, drive to the address of the pointer, move 2 houses to the right, and then with the * get the value in that address

struct user
{
    char username[50];
    char password[50];
    int admin;
};
struct user *createUsers(int max_number_of_users)
{
    return (struct user *)malloc(sizeof(struct user) * max_number_of_users);
}

void initUsers(void *users, int max_number_of_users)
{
    // initializes max number of users users’ usernames to “default”, passwords to “default”, and admins to 0.
    // users.username wont be able to reach the struct and get the username
    // because "void" is not a user struct
    // make sure that the users pointer has already been allocated
    if (users == NULL)
    {
        users = createUsers(max_number_of_users);
    }
    // void pointer says - get a memory address BUT isn't sure what type of variable this points to
    // so not sure how many bytes to move when tring to move a full "house" to the right
    // need to tell C, that this pointer should be a users pointer
    struct user * u = (struct user *)users;
    for (int i = 0; i < max_number_of_users; i++)
    {
        // initialize values to default default and 0
        //u is the address of the first element
        //add i to move on to elements to the right
        //u + 1 is still an address so use * to get the value in that address and now
        // *(u+i) simplifies to a user structure, so use the . to access a value
        strcpy(  (*(u + i)).username  , "default");
        strcpy((*(u + i)).password, "default");
        (*(u + i)).admin = 0;
    }
}

void addUser1(struct user *users, char *username, char *password, int admin, int *count)
{   
    //copies the username, password, and admin
    strcpy( (*(users + *count)).username, username);
    strcpy((*(users + *count)).password, password);
    (*(users + *count)).admin = admin;   
    //add 1 to the user count
    (*count) += 1;

}
void addUser2(struct user *users, struct user *newUser, int *count)
{
    addUser1(users, newUser->username, newUser->password, newUser->admin, count);
}

//Prints to the terminal in the following format
// username0: vahab 
// password0: fgH!12 
void printUser(struct user *users, int max_number_of_users)
{
   for(int i = 0; i < max_number_of_users; i++){
    struct user curUser = *(users+i);
    printf("username%d: %s\n", i,curUser.username);
    printf("password%d: %s\n", i, curUser.password);
   }
}

int main(void)
{

    struct user *users = createUsers(10);
    if (users == NULL)
    {
        return 0;
    }
    int userCount = 0;
    initUsers(users, 10);
    addUser1(users, "admin", "s#1Pa5", 1, &userCount);
    addUser1(users, "vahab", "fgH!12", 0, &userCount);
    struct user u1;
    u1.admin = 2;
    strcpy(u1.username, "usf");
    strcpy(u1.password, "gA24?m");
    addUser2(users, &u1, &userCount);
    printUser(users, userCount);
    free(users);
    return 0;
}

// Implement the following functions. You many not use any square brackets ([]) in your implementation.
// 1. There is a compiler error in main() function. Identity and fix the bug.
// 2. Complete createUsers, which dynamically creates an array of struct user with the length of max number of users,
// and returns the pointer to this array.
// 3. Complete initUsers, which initializes max number of users users’ usernames to “default”, passwords to “default”, and admins to 0.
// 4. Complete addUser1, which adds a new user by passing the username, password, and admin. This function also updates userCount.
// 5. Complete addUser2, which adds a new user by passing struct user. This function also updates userCount.
// 6. Complete printUser, which prints out the username and password for max number of users users. The
// output should follow the example below:
// username0: vahab 
// password0: fgH!12 
// username1: admin
// password1: s#1Pa5