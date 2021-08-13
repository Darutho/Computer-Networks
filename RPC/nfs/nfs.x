/*The IDL File  --- name IDL.x*/

/*Structure to hold the 2 values to be used in computation*/

struct values{
    char fname[10];
};

/*Programme, version and procedure definition*/

program COMPUTE{
    version COMPUTE_VERS{
        int ADD(values) =1;
    } =6;

} = 456123789;
