# SimpleKV
Simple Key-Value database (in memory so far)

#### Usage

```cpp
#include <simplekv.h>

SIMPLEKV::SimpleKV db;
```

#### Basic type

```cpp
db.Add<int>("Age", 666);
printf("Age: %d\n", db.Get("Age")->value<int>());
// > Age: 666

db.Put<int>("Age", -777);
printf("Age: %d\n", db.Get("Age")->value<int>());
// > Age: -777

db.Add<float>("float", 6.666);
db.Add<double>("double", 2333.3333333333);

printf("%f\n", db.Get("float")->value<float>());
// > 6.666000
printf("%.10f\n", db.Get("double")->value<double>());
// > 2333.3333333333
```

#### Array

```cpp
uint16_t data[10] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27};
db.Add("Data", data);

for (int i = 0; i < 10; i++) {
    printf("%d ", db.Get("Data")->value<uint16_t*>()[i]);
}
printf("\n");
// > 0 3 6 9 12 15 18 21 24 27
```

#### Custom type

```cpp
struct MyType_t {
    int a;
    float b;
    std::string status;
};
MyType_t my_type_1 = {11, 1.11, "nihao"};
MyType_t my_type_2 = {-22, -2.22, "wohao"};
MyType_t my_type_temp = {0};

db.Add("MyType_1", (void*)&my_type_1, sizeof(MyType_t));
db.Add("MyType_2", (void*)&my_type_2, sizeof(MyType_t));

my_type_temp = db.Get("MyType_1")->value<MyType_t>();
printf("%d %.2f %s\n", my_type_temp.a, my_type_temp.b, my_type_temp.status.c_str());
// > 11 1.11 nihao

my_type_temp = db.Get("MyType_2")->value<MyType_t>();
printf("%d %.2f %s\n", my_type_temp.a, my_type_temp.b, my_type_temp.status.c_str());
// > -22 -2.22 wohao
```

```cpp
/* << Bug >>: Add like this causes string become the same, don't know why */
db.Add("MyType_3", my_type_1);
db.Add("MyType_4", my_type_2);

my_type_temp = db.Get("MyType_3")->value<MyType_t>();
printf("%d %.2f %s\n", my_type_temp.a, my_type_temp.b, my_type_temp.status.c_str());
// > 11 1.11 wohao

my_type_temp = db.Get("MyType_4")->value<MyType_t>();
printf("%d %.2f %s\n", my_type_temp.a, my_type_temp.b, my_type_temp.status.c_str());
// > -22 -2.22 wohao
```

