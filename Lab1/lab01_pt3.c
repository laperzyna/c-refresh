#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define READ 2
#define WRITE 1
#define EXECUTE 0
#define EXIT_SUCCESS 0

// 1. Consider the struct user used in Part I.In this part, we modified it slightly.
//Instead of admin, we use three Boolean members to determine whether a user has proper permissions to write, read and execute files.
struct new_user {
    char username[50];
    char password[50];
    bool write;
    bool read;
    bool execute;
};

// We can reduce the size of each new user by using one unsigned char to store up to 8 Boolean values instead
// of three Boolean variables : 

typedef unsigned char bitmap8; //0 to 255
struct user
{
    char username[50];
    char password[50];
    bitmap8 permissions;
};
//unsigned char is 1 byte - 1 byte is 8 bits
//in order to make up this one byte there are 8 0s or 1s
//00000000
//the first 3 bits are going to tell the permissions
//first bit is execute --> 00000001    this user has permission to execute
//second bit is write --> 00000010    this user has permission to write
//third bit is read --> 00000100    this user has permission to read

//Ex: 00000101   what permissions does this used have?  read && execute

//int -35767 to 35767   total numbers  ~70000  unsigned it shifts the range --> 0 - 70000 
//unsigned int test = 70000;
//test += 1;  //the value of test would wrap around to 0

//The first bit of the permissions indicates execute permission, while the second and third bits indicate write and read permissions, 
//respectively. Call the function compare() to see how much memory we saved by using a bitmap instead of three Boolean variables.
void compare() {
    printf("The size of new_user is %d.\n", sizeof(struct new_user));
    printf("The size of user is %d.\n", sizeof(struct user));
}

// Since we use one char to store three permissions, we need three helper functions to authorize, revoke and check permissions for a user. 
// In this lab, you are to complete the three functions grantPermission, revokePermission and checkPermission. 
// In these functions, bitIndex refers to the index of the bit. For example, if a user has only bitIndex 1 authorized, the binary format 
// of permissions in user is 0000 0010. 
//If a user has bitIndex 1 and 0 authorized, the binary format of permissions in user is 0000 0011.

void grantPermission(int bitIndex, struct user* user) { 
    //bit index is a number from 0 to 2 that represents which bit we are changing

    //assuming the permission is 0 right now
    //and bit index is 1
    // user's current permissions           0000 0101 --> 0000 0010
    // this number is based on bitIndex  or 0000 0010
    //                                            111

    //bit shifting - move all bits in a number either left or right
    //left most bit is most significant and right most bit is least significant

    // right shift    >>   as you shift to the right the new bit that comes in is the same as the most significant bit
    // ex:   100 >> 2   111          ex2:   010 >> 2  =  000

    //left shift  <<   as you shift to the left the new bit IS ALWAYS A 0
    // ex:  010 << 1 = 100       ex:2  001 << 3 = 000

    //first make the number to OR with
    int orValue = 1 << bitIndex; // 100

    (*user).permissions = (*user).permissions | orValue;    
}

void revokePermission(int bitIndex, struct user *user) { 
    //this should set the correct bit to a 0

    //curPermission  0000 0111
    //andValue       1111 1101
    //newPermissioni 0000 0101

    //to get the and value should first make
    //0000 0010 then use ~ to NOT it
    //1111 1101
    int andValue = ~(1 << bitIndex); //0000 0001
    (*user).permissions = (*user).permissions & andValue;
}

int checkPermission(int bitIndex, struct user *user) { 
    //user.permission is 0000 0010
    //need to check to see if the 'bitIndex' bit is HIGH

    //     0000 0010
    //and  0000 0010
    //     0000 0010

    int andValue = 1 << bitIndex;
    int curPermission = (*user).permissions & andValue;
    //at this point the curPermission can only have 2 possible values the permission is either granted or not
    //granted     0000 0010
    //not granted 0000 0000
    //only care about the value in the bitIndex of 1

    //this moves the primary bit into the ones position so it will return either a 1 or 0
    //curPermission    0000 0010
    //after the shift  0000 0001
    return curPermission >> bitIndex;

    //Example when permission is not granted
    //user permission 0000 101 
    //bit index is 1

    //andValue 0000 0010

    //user permission 0000 0101 
    //andValue        0000 0010
    //current         0000 0000
    //0000 0000 >> 2  0000 0000 
}

void setPermissions(int new_permissions, struct user* user) {
//new permissions is an integer -> think of like a binary
    // 7 - > 0000 0111

    //check if read is high
    //
    //and       0000 0001
    //if the answer is 1 it's high
    int andValue = 1;
    int exectueVal = new_permissions & andValue;
    if (exectueVal){
        grantPermission(EXECUTE, user);
    } else {
        revokePermission(EXECUTE, user);
    }

    //check if write is high
    //and       0000 0010
    //if after bit shifting the answer is 1 then its high
    andValue = 1 << WRITE;
    //anding will give 0000 0010 we still have to shift it right so we can tell if its a 1 or 0
    int writeVal = (new_permissions & andValue) >> WRITE;
    if (writeVal){
        grantPermission(WRITE, user);
    } else {
        revokePermission(WRITE, user);
    }

     //check if execute is high
    //and       0000 0100
    //if after bit shifting the answer is 1 then its high
    andValue = 1 << READ;
    //anding will give 0000 0010 we still have to shift it right so we can tell if its a 1 or 0
    int readVal = (new_permissions & andValue) >> READ;
    if (readVal){
        grantPermission(READ, user);
    } else {
        revokePermission(READ, user);
    }
}

void printPermissions(struct user *user) {
    if (checkPermission(WRITE, user)) {
        printf("%s has write permission.\n", user->username);
    } else {
        printf("%s doesn't have write permission.\n", user->username);
    }

    if (checkPermission(READ, user)) {
        printf("%s has read permission.\n", user->username);
    } else {
        printf("%s doesn't have read permission.\n", user->username);
    }

    if (checkPermission(EXECUTE, user)) {
        printf("%s has execute permission.\n", user->username);
    } else {
        printf("%s doesn't have execute permission.\n", user->username);
    }
    printf("\n");
}

int main(void) {

    struct user user;
    strcpy(user.username, "admin");
    strcpy(user.password, "s#1Pa5");
    user.permissions = 0; // Sets the permissions to 0 grantPermission(0, &user);
    grantPermission(WRITE, &user);   
    printPermissions(&user);
    revokePermission(WRITE, &user);
    grantPermission(EXECUTE, &user);
    printPermissions(&user);


    printf("Set all with a 7\n");
    setPermissions(7,&user);
    printPermissions(&user);
    //read write execute
    printf("Set read and execute with a 5\n"); //101
    setPermissions(5,&user);
    printPermissions(&user);

    printf("Set write and read with a 6\n"); //110
    setPermissions(6,&user);
    printPermissions(&user);

    printf("Set write and execute with a 3\n"); //011
    setPermissions(3,&user);
    printPermissions(&user);

    printf("turn all off with 0\n"); //011
    setPermissions(0,&user);
    printPermissions(&user);

    return EXIT_SUCCESS;
}

// 2. Implement setPermissions for Problem 1 in Part III. This function is similar to command chmod.
// In Unix, each file has read, write and execute permissions, similar to the permissions of sturct user, except that
// the file has three groups of permissions for owner, group and others. This is the basis of the Unix (POSIX) access control model.
// In binary format, 4 is equal to 0000 0100, which indicates read permission. 2 is equal to 0000 0010, which indicates write permission.
// 1 is equal to 0000 0001, which indicates execute permission. We can use three numbers to set the permissions for the three groups using
// chmod:
//          $ chmod 444 file name
// This command sets read permissions for each group. You can also set multiple permissions in one command.
// For instance, the binary format of 7 is 0000 0111, so execute chomd with 7 sets all read, write and execute permissions:
//          $ chmod 777 file
// The function setPermissions does the same thing. It sets the permissions of user using the given permis-
// sions. You are to implement the function setPermissions that takes a number between 0 and 7 and sets or 5
// revokes permissions according to new permissions .
//      void setPermissions(int new_permissions, struct user* user) {
//          // Complete me
//      }
//  3 -> 011
//  6 -> 101

//  5Notice that new permissions and user->permissions are not the same data types.
