# cmn_cpp_tool
*1*主要都是收集些工具或是好用的cpp类，大部分来源于个人博客上面，我会尽力标注
出处和原作者，如果原作者看到有不满意的请联系我删除。
谢谢：

#cmn_cpp_tool
##common_class
*1Any  
用法：
`
    struct AA s_a;
    Any n = s_a;    
    struct AA s_b = n.AnyCast<AA>();
` 
>来源于[qicosmos(江南)](http://www.cnblogs.com/qicosmos/p/3420095.html)

*2MessageBus