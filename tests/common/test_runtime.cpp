// Stubs console pause/system calls so menu-driven tests do not block.
extern "C" {

int __wrap_getch(void)
{
    return '\r';
}

int __wrap__getch(void)
{
    return '\r';
}

int __wrap_system(const char* command)
{
    (void)command;
    return 0;
}

}
