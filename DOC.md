segmentation.cpp:
提供的接口：

    std::vector<Utf8String> Segmentation::segment(const Utf8String & sentence)
功能:
    
    传入一个UTF-8编码的字符串，将这个字符串按照中文语义切割，返回一个中文词向量
算法:
    
    1.按词频词典找出中文字串中存在的歧义区间
    
    2.歧义区间中选出若干种词的组合，每种组合要消除歧义
    
    3.计算出选出的组合中信息量最小的，这个组合可以认为是最接近语义的组合
    
    如 记者读报纸
    分割的结果是 记者 读 报纸
    
searchserver.cpp:

提供的类: 

    Server 

提供的接口:

    Server::listen()
    Server::run()
功能:

    侦听来自前端服务器的检索请求，将检索结果返回给前端服务器
    每个请求用一个线程处理
