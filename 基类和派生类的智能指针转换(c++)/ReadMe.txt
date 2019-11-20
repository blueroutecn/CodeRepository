基类和派生类的智能指针转换（c++）:

KeyWords: static_pointer_cast, static_cast, dynamic_cast, auto, shared_ptr.


/* ------ pointer_casters.h ------*/
1. 实现智能指针shared_ptr在基类和派生之间的转换，包括
（派生类指针 上行   转基类指针）
（基类指针    下行   转派生类指针）
（基类指针    下行   转派生类对象）