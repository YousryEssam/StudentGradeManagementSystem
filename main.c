#include <stdio.h>
#include <stdlib.h>
#include<windows.h>

// Display Part
typedef enum
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
} ConsoleColor;
void setConsoleColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = (bgColor << 4) | textColor;
    SetConsoleTextAttribute(hConsole, colorAttribute);
}
int get_console_width()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return -1;
}
void draw_menu(char* arr[], int len, int cur_opt)
{
    for(int i = 0 ; i < len ; i ++ )
    {
        if(i == cur_opt) setConsoleColor(RED, BLACK);
        printf("%d - %s \n", i + 1, arr[i]);
        if(i == cur_opt) setConsoleColor(WHITE, BLACK);
    }
}
void close_console_app()
{
    printf("Exiting the application...\n");
    exit(0); // 0 indicates successful termination
}
// input processing part
int get_input()
{
    char input = getch();
    // Extended Key
    if(input == -32)
    {
        input = getch();
        if(input == 72)
        {
            // up
            return -1;
        }
        else if(input == 80 )
        {
            // down
            return 1;
        }
        else if(input == 75)
        {
            // left
            return 0;
        }
        else if (input == 77)
        {
            // right
            return 0;
        }
    }
    else
    {
        if(input == 13)
        {
            // enter key
            return 2;
        }
    }
}
// Data Structures
const int subject_names_length = 13;
const char* subject_names[] =
{
    "C", "C++", "OOP", "HTML", "CSS", "JAVASCRIPT",
    "PROGRAMMING","DATA_STRUCTURES", "ALGORITHMS","DATABASES",
    "OPERATING_SYSTEMS", "COMPUTER_NETWORKS","SOFTWARE_ENGINEERING"
};
const int main_menu_length = 5;
const char* main_menu[] =
{
    "Add new Student",
    "Update Student Data",
    "Display Student",
    "System Statistics",
    "Exit"
};
const int update_menu_length= 4;
const char* update_menu[] =
{
    "Name",
    "ID",
    "Courses",
    "Back"
};

const int display_menu_length = 3;
const char* display_menu[] =
{
    "Display student by ID.",
    "Display All Students.",
    "Back"
};

const int system_statistics_length = 3;
const char* system_statistics[] =
{
    "pass Students",
    "fail Students",
    "Back"
};

const int courses_update_menu_length = 4;
const char* courses_update_menu[] =
{
    "Add a new course",
    "Update an existing course",
    "Remove a course",
    "Back"
};

struct Course
{
    int id;
    int score;
    char grade;
};
typedef struct Course Course;

struct Student
{
    int id;
    char name[50];
    int num_of_courses;
    Course *courses;
};
typedef struct Student Student;
// Data store
Student *students;
int students_lenght = 0,students_next_idx = 0;
// Functions
char calc_grade( int score )
{
    if (score < 60)
    {
        return 'F';
    }
    else if (score < 70)
    {
        return'D';
    }
    else if (score < 85)
    {
        return 'C';
    }
    else if (score < 90)
    {
        return 'B';
    }
    else
    {
        return 'A';
    }
}
float student_average(Student st)
{
    if (st.num_of_courses == 0)
    {
        return 0; // Avoid division by zero
    }

    float total_score = 0;
    for (int i = 0; i < st.num_of_courses; i++)
    {
        total_score += st.courses[i].score;
    }

    return total_score / st.num_of_courses; // Correct division
}
int is_pass_student(Student st)
{
    return student_average(st) >= 60 ;
}
int top_performer(Student st)
{
    int mx =0, idx = -1 ;
    for(int i = 0 ; i < st.num_of_courses ; i ++ )
    {
        if(st.courses[i].score >= mx)
        {
            mx = st.courses[i].score;
            idx = st.courses[i].id;
        }
    }
    return idx;
}
char student_grade(Student st)
{
    int score = student_average(st);
    if (score < 60)
    {
        return 'F';
    }
    else if (score < 70)
    {
        return'D';
    }
    else if (score < 85)
    {
        return 'C';
    }
    else if (score < 90)
    {
        return 'B';
    }
    else
    {
        return 'A';
    }
}
int valid_score(int score )
{
    return score >= 0 && score <= 100;
}
Course add_course()
{
    Course course;
    int current_option = 0;

    while (1)
    {
        system("cls");
        draw_menu((char**)subject_names, subject_names_length, current_option);
        printf("\nSelect the course Please.\n");
        int input = get_input();
        if (input == -1)
        {
            current_option = (current_option - 1 + subject_names_length) % subject_names_length;
        }
        else if (input == 1)
        {
            current_option = (current_option + 1) % subject_names_length;
        }
        else if (input == 2)
        {
            course.id = current_option;
            break;
        }
    }
    system("cls");
    printf("You selected: %s\n", subject_names[course.id]);
    printf("Enter course score: ");
    scanf("%d", &course.score);
    system("cls");
    while(!valid_score(course.score) )
    {
        printf("You selected: %s\n", subject_names[course.id]);
        printf("The Score you entered is invalid, Reenter course score: ");
        scanf("%d", &course.score);
        system("cls");
    }
    course.grade = calc_grade(course.score);
    return course;
}
void extend_students_array()
{
    if(!students_lenght)
    {
        students_lenght++;
    }
    else
    {
        students_lenght *=2;
    }

    Student* new_arr = (Student*)malloc(students_lenght * sizeof(Student));
    if (new_arr == NULL)
    {
        printf("Memory allocation failed for students array.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < students_next_idx; i++)
    {
        new_arr[i].id = students[i].id;
        new_arr[i].num_of_courses = students[i].num_of_courses;
        strcpy(new_arr[i].name, students[i].name);
        new_arr[i].courses = (Course*)malloc(students[i].num_of_courses * sizeof(Course));
        if (new_arr[i].courses == NULL)
        {
            printf("Memory allocation failed for courses.\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < students[i].num_of_courses; j++)
        {
            new_arr[i].courses[j] = students[i].courses[j];
        }
        free(students[i].courses);
    }
    free(students);
    students = new_arr;
}
void add_new_student()
{
    system("cls");
    if (students_lenght == students_next_idx)
    {
        extend_students_array();
    }

    printf("Please enter student ID: ");
    scanf("%d", &students[students_next_idx].id);

    printf("Please enter student Name: ");
    scanf("%s", students[students_next_idx].name);

    printf("Please enter the number of courses for the student: ");
    scanf("%d", &students[students_next_idx].num_of_courses);

    students[students_next_idx].courses = (Course*)malloc(students[students_next_idx].num_of_courses * sizeof(Course));
    if (students[students_next_idx].courses == NULL)
    {
        printf("Memory allocation failed for courses.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < students[students_next_idx].num_of_courses; i++)
    {
        printf("Adding course %d:\n", i + 1);
        students[students_next_idx].courses[i] = add_course();
    }

    students_next_idx++;
}
void display_courses(Course *courses, int len)
{
    if (len == 0)
    {
        printf("No courses available.\n");
        return;
    }

    printf("\nCourse Details:\n");
    printf("================================================\n");

    printf("| %-4s | %-20s | %-5s | %-6s |\n", "ID", "Name", "Score", "Grade");
    printf("================================================\n");

    for (int i = 0; i < len; i++)
    {
        printf("| %-4d | %-20s | %-5d | %-6c |\n",
               courses[i].id,
               (courses[i].id >= 0 && courses[i].id < subject_names_length) ? subject_names[courses[i].id] : "Unknown",
               courses[i].score,
               courses[i].grade);
    }
    printf("================================================\n");
}
void display_student(Student student)
{
    float std_avg = student_average(student);
    char grade = student_grade(student);
    int top_performer_course = top_performer(student);
    printf("=============================================================\n");
    printf("|                     Student Information                   |\n");
    printf("=============================================================\n");
    printf("| %-20s : %-34d |\n", "ID", student.id);
    printf("| %-20s : %-34s |\n", "Name", student.name);
    printf("| %-20s : %-34d |\n", "Number of Courses", student.num_of_courses);
    printf("| %-20s : %-34.2f |\n", "Average Score", std_avg);
    printf("| %-20s : %-34c |\n", "Total Grade", grade);
    printf("| %-20s : ", "Top Performer");
    if (top_performer_course != -1)
    {
        printf("%-34s |\n", subject_names[top_performer_course]);
    }
    else
    {
        printf("%-34s |\n", "None");
    }
    // "                                          ||"
    printf("=============================================================\n");
    printf("\nEnrolled Courses:\n");
    display_courses(student.courses, student.num_of_courses);
    printf("\nPress any key to return...\n");
    getch();
}
void display_all_students()
{
    system("cls");

    if (students_next_idx == 0)
    {
        printf("=============================================================\n");
        printf("|                 No Students in the System                 |\n");
        printf("=============================================================\n");
        printf("Press any key to return...\n");
        getch();
        return;
    }

    for (int i = 0; i < students_next_idx; i++)
    {
        float std_avg = student_average(students[i]);
        char grade = student_grade(students[i]);
        int top_performer_course = top_performer(students[i]);

        printf("=============================================================\n");
        printf("|                   Student Number %d                        |\n", i + 1);
        printf("=============================================================\n");
        printf("| %-20s : %-34d |\n", "ID", students[i].id);
        printf("| %-20s : %-34s |\n", "Name", students[i].name);
        printf("| %-20s : %-34d |\n", "Number of Courses", students[i].num_of_courses);
        printf("| %-20s : %-34.2f |\n", "Average Score", std_avg);
        printf("| %-20s : %-34c |\n", "Total Grade", grade);
        printf("| %-20s : ", "Top Performer");

        if (top_performer_course != -1)
        {
            printf("%-34s |\n", subject_names[top_performer_course]);
        }
        else
        {
            printf("%-34s |\n", "None");
        }

        printf("=============================================================\n");
        printf("\nEnrolled Courses:\n");
        display_courses(students[i].courses, students[i].num_of_courses);
        printf("\n");
    }

    printf("Press any key to return...\n");
    getch();
    system("cls");
}
void display_student_menu()
{
    int current_option = 0;
    while (1)
    {
        system("cls");
        printf("Student Display Menu:\n");
        draw_menu((char**)display_menu, display_menu_length, current_option);

        int input = get_input();
        if (input == -1)
        {
            current_option = (current_option - 1 + display_menu_length) % display_menu_length;
        }
        else if (input == 1)
        {
            current_option = (current_option + 1) % display_menu_length;
        }
        else if (input == 2)
        {
            switch (current_option)
            {
            case 0:
            {
                system("cls");
                printf("Enter the Student ID: ");
                int student_id;
                scanf("%d", &student_id);
                int found = 0;
                for (int i = 0; i < students_next_idx; i++)
                {
                    if (students[i].id == student_id)
                    {
                        display_student(students[i]);
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    printf("Student with ID %d not found.\n", student_id);
                    printf("Press any key to return to the menu...\n");
                    getch();
                }
                break;
            }
            case 1:
                display_all_students(students, students_next_idx);
                break;
            case 2:
                system("cls");
                return;
            }
        }
    }
}
void update_name(int idx )
{
    printf("Enter new name for student (current: %s): ", students[idx].name);
    scanf(" %[^\n]", students[idx].name);
    setConsoleColor(GREEN, BLACK );
    printf("Name updated successfully!\n");
    setConsoleColor(WHITE, BLACK );
    printf("Press any Key to continue ......\n");
    getch();
}
void update_id(int idx )
{
    printf("Enter new ID for student (current: %d): ", students[idx].id);
    scanf("%d", &students[idx].id);
    setConsoleColor(GREEN, BLACK );
    printf("ID updated successfully!\n");
    setConsoleColor(WHITE, BLACK );
    printf("Press any Key to continue ......\n");
    getch();
}
void update_courses_add_new(int idx)
{
    printf("Adding a new course...\n");
    students[idx].num_of_courses++;
    students[idx].courses = realloc(
                                students[idx].courses,
                                students[idx].num_of_courses * sizeof(Course)
                            );
    students[idx].courses[students[idx].num_of_courses - 1] = add_course();
    setConsoleColor(GREEN, BLACK );
    printf("Course added successfully!\n");
    setConsoleColor(WHITE, BLACK );
    printf("Press any Key to continue ......\n");
    getch();
}
void update_courses_modifi(int idx)
{
    printf("Enter the course ID to update: ");
    int course_id;
    scanf("%d", &course_id);
    int found = 0;
    for (int i = 0; i < students[idx].num_of_courses; i++)
    {
        if (students[idx].courses[i].id == course_id)
        {
            printf("Updating course ID %d:\n", course_id);
            students[idx].courses[i] = add_course();
            found = 1;
            break;
        }
    }
    if (!found)
    {
        setConsoleColor(YELLOW, BLACK );
        printf("Course ID %d not found.\n", course_id);
        setConsoleColor(WHITE, BLACK );
        printf("Press any Key to continue ......\n");
        getch();
        return ;
    }
    setConsoleColor(GREEN, BLACK );
    printf("Course updated successfully!\n");
    setConsoleColor(WHITE, BLACK );
    printf("Press any Key to continue ......\n");
    getch();
}
void update_courses_remove(int idx)
{

    printf("Enter the course ID to remove: ");
    int course_id;
    scanf("%d", &course_id);
    int found = 0;
    for (int i = 0; i < students[idx].num_of_courses; i++)
    {
        if (students[idx].courses[i].id == course_id)
        {
            for (int j = i; j < students[idx].num_of_courses - 1; j++)
            {
                students[idx].courses[j] = students[idx].courses[j + 1];
            }
            students[idx].num_of_courses--;
            students[idx].courses = realloc(
                                        students[idx].courses,
                                        students[idx].num_of_courses * sizeof(Course)
                                    );
            found = 1;
            setConsoleColor(GREEN, BLACK );
            printf("Course removed successfully!\n");
            setConsoleColor(WHITE, BLACK );
            printf("Press any Key to continue ......\n");
            getch();
            return ;
        }
    }
    if (!found)
    {
        setConsoleColor(YELLOW, BLACK );
        printf("Course ID %d not found.\n", course_id);
        setConsoleColor(WHITE, BLACK );
        printf("Press any Key to continue ......\n");
        getch();
        return ;
    }
}
void update_courses_menu(int idx)
{

    int in_menu = 1;
    int cur_opt = 0 ;
    do
    {
        system("cls");
        printf("Current courses for the student:\n");
        display_courses(students[idx].courses, students[idx].num_of_courses);
        printf("Do you want to:\n");
        draw_menu( courses_update_menu, courses_update_menu_length, cur_opt);
        int user_input = get_input();

        if(user_input == 2)
        {
            switch(cur_opt)
            {
            case 0 :
                update_courses_add_new(idx);
                break;
            case 1 :
                update_courses_modifi(idx);
                break;
            case 2 :
                update_courses_remove(idx);
                break;
            case 3 :
                in_menu = 0;
                break;
            }
        }
        else
        {
            cur_opt = cur_opt + user_input + courses_update_menu_length;
            cur_opt %= courses_update_menu_length;

        }

    }
    while(in_menu);

}
void update_student()
{

    system("cls");
    printf("Please enter student ID: ");
    int student_id;
    scanf("%d", &student_id);
    int student_idx = -1;
    for (int i = 0; i < students_next_idx; i++)
    {
        if (students[i].id == student_id)
        {
            student_idx = i;
            break;
        }
    }
    if (student_idx == -1)
    {
        printf("Student with ID %d not found.\n", student_id);
        printf("Press any key to return to the main menu...\n");
        getch();
        return;
    }

    int current_option = 0;
    while (1)
    {
        system("cls");
        printf("\nSelect what you want to update:\n");
        draw_menu((char **)update_menu, update_menu_length, current_option);

        int input = get_input();
        if(input == 2)
        {
            // Confirm selection
            system("cls");

            if (current_option == 0)
            {
                update_name(student_idx);
            }
            else if (current_option == 1)
            {
                update_id(student_idx);
            }
            else if(current_option == 2)
            {

                update_courses_menu(student_idx);
            }

            else
            {
                printf("Press any key to continue...\n");
                getch();
                break;
            }
        }
        else
        {
            current_option = (current_option + input + update_menu_length ) % update_menu_length;
        }
    }
}
void passed_students()
{
    int idx = 1;
    for(int i = 0  ; i< students_next_idx ; i ++  )
    {
        if(is_pass_student(students[i]))
        {
            printf("=============================%d===============================\n", idx ++);
            display_student(students[i]);
        }
    }
    printf("Press any key to back.........\n");
    getch();
    system("cls");

}
void failed_students()
{
    int idx = 1;
    for(int i = 0  ; i< students_next_idx ; i ++  )
    {
        if(!is_pass_student(students[i]))
        {
            printf("=============================%d===============================\n", idx ++);
            display_student(students[i]);
        }
    }
    printf("Press any key to back.........\n");
    getch();
    system("cls");

}
void system_statistics_menu()
{
    int in_menu = 1;
    int cur_opt = 0 ;
    do
    {
        system("cls");
        draw_menu( system_statistics, system_statistics_length, cur_opt);
        int user_input = get_input();
        if(user_input == 2)
        {
            switch(cur_opt)
            {
            case 0 :
                passed_students();
                break;
            case 1 :
                failed_students();
            case 2 :
                in_menu = 0;
                break;
            }
        }
        else
        {
            cur_opt = cur_opt + user_input + system_statistics_length;
            cur_opt %= system_statistics_length;

        }

    }
    while(in_menu);

}
int main()
{
    int exit_program = 0 ;
    int current_option = 0;
    do
    {
        system("cls");
        draw_menu(main_menu,main_menu_length, current_option);
        int user_input = get_input();
        if(user_input == 2)
        {
            switch(current_option)
            {
            case 0:
                add_new_student();
                break;
            case 1:
                update_student();
                break;
            case 2:
                display_student_menu();
                break;
            case 3 :
                system_statistics_menu();
                break;
            case 4 :
                close_console_app();
                break;

            }
        }
        else
        {
            current_option += user_input;
            current_option = (current_option + main_menu_length) % main_menu_length;
        }
    }
    while(!exit_program);
    return 0;
}
