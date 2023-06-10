# <center>Lec08作业</center>

## <center>ZY2006109-姬轶</center>

</br>

### 在IMP语言中增加取值和寻址表达式、修改指针指向的内容，其语法进行了如下扩充：
```java
Command ::=  *Identifier =  Expression
Expression :: = *Identifier
       | &Identifier
```
> 解：
> ```
> execute [*I = E] env sto =
>       let val = evaluate E env sto in
>       let loc = find(env, I) in
>           let locI = feach(sto, loc) in
>               sto(locI)
>       update(env, sto loc)
> 
> evaluation [*I] env sto =
>       let loc = find(env, I) in
>           let locI = feach(sto, loc) in
>               sto(locI)
> 
> evaluation [&I] env sto =
>       find(env, I)
> ```