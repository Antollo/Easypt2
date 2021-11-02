#define insertObject(NAME, ...) st->insert(NAME, object::makeObject(__VA_ARGS__))
#define insertFunctionL(NAME, ...) st->insert(NAME, object::makeObject(static_cast<object::type::NativeFunction>([](object::objectPtr thisObj, object::type::Array && args, stack * st) -> object::objectPtr __VA_ARGS__)))
#define addFunctionL(OBJECTPTR, NAME, ...) OBJECTPTR->addProperty(NAME, object::makeObject(static_cast<object::type::NativeFunction>([](object::objectPtr thisObj, object::type::Array && args, stack * st) -> object::objectPtr __VA_ARGS__)))
#define addFunctionL2(OBJECTPTR1, OBJECTPTR2, NAME, ...)                                                                                                                                        \
    {                                                                                                                                                                                           \
        auto function = object::makeObject(static_cast<object::type::NativeFunction>([](object::objectPtr thisObj, object::type::Array && args, stack * st) -> object::objectPtr __VA_ARGS__)); \
        OBJECTPTR1->addProperty(NAME, function);                                                                                                                                                \
        OBJECTPTR2->addProperty(NAME, function);                                                                                                                                                \
    }