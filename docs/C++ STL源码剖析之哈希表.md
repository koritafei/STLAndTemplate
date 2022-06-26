`hashtable`源码
```cpp
template<typename _Key, typename _Value, typename _Alloc,
    typename _ExtractKey, typename _Equal,
    typename _H1, typename _H2, typename _Hash,
    typename _RehashPolicy, typename _Traits>
class _Hashtable
: 　public __detail::_Hashtable_base<_Key, _Value, _ExtractKey, _Equal,
    public __detail::_Map_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,  _H1, _H2, _Hash, _RehashPolicy, _Traits>,
    public __detail::_Insert<_Key, _Value, _Alloc, _ExtractKey, _Equal, _H1, _H2, _Hash, _RehashPolicy, _Traits>,
    public __detail::_Rehash_base<_Key, _Value, _Alloc, _ExtractKey, _Equal,_H1, _H2, _Hash, _RehashPolicy, _Traits>,
    public __detail::_Equality<_Key, _Value, _Alloc, _ExtractKey, _Equal,_H1, _H2, _Hash, _RehashPolicy, _Traits>,
    private __detail::_Hashtable_alloc<typename __alloctr_rebind<_Alloc,__detail::_Hash_node<_Value,_Traits::__hash_cached::value> >::__type>
{
};
```
帮助程序类，将仿函数_Equal的管理添加到_Hash_code_base中。
对比代码就可以看出来是啥意思了：
```cpp
template<typename _Key, typename _Value,
    typename _ExtractKey, typename _Equal,
    typename _H1, typename _H2, typename _Hash, typename _Traits>
struct _Hashtable_base
: public _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, _Traits::__hash_cached::value>,
private _Hashtable_ebo_helper<0, _Equal> {
};
```
`_Hash_code_base`
```cpp
template<typename _Key, typename _Value, typename _ExtractKey,
typename _H1, typename _H2, typename _Hash,bool __cache_hash_code>
struct _Hash_code_base;


template<typename _Key, typename _Value, typename _ExtractKey,
typename _H1, typename _H2, typename _Hash>
struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2, _Hash, false>
: private _Hashtable_ebo_helper<0, _ExtractKey>,
private _Hashtable_ebo_helper<1, _Hash>
}

template<typename _Key, typename _Value, typename _ExtractKey,
typename _H1, typename _H2>
struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2,
_Default_ranged_hash, false>
: private _Hashtable_ebo_helper<0, _ExtractKey>,
private _Hashtable_ebo_helper<1, _H1>,
private _Hashtable_ebo_helper<2, _H2>
{
};

template<typename _Key, typename _Value, typename _ExtractKey,
    typename _H1, typename _H2>
struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2,
            _Default_ranged_hash, true>
: private _Hashtable_ebo_helper<0, _ExtractKey>,
    private _Hashtable_ebo_helper<1, _H1>,
    private _Hashtable_ebo_helper<2, _H2>
{
```

