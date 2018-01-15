## Debug Pitfalls

As described in this [issue](https://bugs.openjdk.java.net/browse/JDK-8136586), Java VM take it as normal to signal 
SIGSEGV null reference access in Java code during startup, which is converted to a NullPointerException and then caught
and handled by Java code to adjust and continue. Yet, it still imposes troubles while debugging JNI native code. 
The following command help suppress and ignore such signals:

* Clang LLDB:

```bash
    process handle SIGSEGV --notify true --pass true --stop false
```

* GDB 

```bash
    handle SIGSEGV nostop
```
