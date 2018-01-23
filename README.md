## Introduction
This project contains two modules: OpenMessaging C++ Specification API and JNI-based implementation.

For the first part, we are intending to maintain conformance to Java counterpart. 

For the latter one, a proxy is planned to implement, embedding a JVM and delegating all invocation to Java SDK. Make 
sure you have thoroughly understood JNI technique before contributing. [JNI Programmer Guide](doc/JNI_guide.pdf) is by 
far the best resource to consult.


## Documentation

1. [Application Design Specification](doc/ApplicationDesignSpecification.md)
2. [Avoid Debugging Pitfalls Guide](doc/Debug.md)
3. [JNI Programmer Guide](doc/JNI_guide.pdf)