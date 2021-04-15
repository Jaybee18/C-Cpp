template <class contentType> class obj
{
public:
    contentType a;
    next
    obj()
    {

    }
};

template <class contentType>
class plist
{
public:
    obj<contentType> head;
    plist()
    {

    }
};