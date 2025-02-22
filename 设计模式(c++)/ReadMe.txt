设计模式（C++）

/*------- DesignModel.h  DesignModel.cpp --------*/


设计原则 & 设计模式（总纲）

1.1 创建型：单例模式 简单工厂模式 工厂方法模式 抽象工厂模式 建造者模式 原型模式；

1.2 结构型：代理模式 适配器模式 装饰器模式 桥接模式 组合模式 享元模式 外观模式；
    结构型类模式，采用继承机制来组合接口和实现；结构型对象模式，采用对象的组合来实现新功能。

1.3 行为型： 观察者模式 模板方法模式 命令模式 状态模式 职责链模式 解释器模式 中介者模式 访问者模式 策略模式 备忘录模式 迭代器模式



如何去理解一个设计模式

2.1 面向对象：说明设计模式主要面向类还是对象。

2.2 常用场景：描述设计模式大部分情况下都会在哪些场景下出现。

2.3 变化点：  说明设计模式容纳哪些因素的变化。

2.4 选择关键点：当选择使用一个设计模式的时候，指出最关键的选择点在哪里。

2.5 模式缺陷：说明设计模式的主要缺陷（无法解决的问题）。

2.6 相关设计模式：与其它相似设计模式的关系。



结构型设计模式

3.1 适配器模式
面向对象：面向类，或者对象。
常用场景：需要使用一个类的功能，但是该类的接口不符合使用场合要求的接口，可使用适配器模式。
变化点：  无（适配器是一对一定制的，不适应变化）。
选择关键点：  当前需求有类似接口可以实现。
模式缺陷：    需要对每一个现存类或对象定制适配器。
相关设计模式：装饰器模式
对于适配器模式中的定制适配器与装饰器模式，二者都是使用组合加继承的手段，不同的是，适配器模式的目的在于适配接口，
装饰器模式的目的在于动态的添加功能，且可以叠加。

3.2 装饰器模式
面向对象：对象。
常用场景：一个类的具体对象需要动态的添加功能，且这些功能可以相互叠加。
变化点：  动态添加的功能（所以装饰器需要抽象出来一层）。
选择关键点：  添加的功能是否需要动态组装。
模式缺陷：    装饰模式增加了许多子类，如果过度使用会使程序变得很复杂。
相关设计模式：适配器模式

3.3 组合模式
面向对象：对象。
常用场景：将对象组合成树状的层次结构，且向客户端提供一致的操作接口，使用户对单个对象和组合对象具有一致的访问性。
变化点：  节点的数量。
选择关键点：对外提供一致操作接口的结构是否可转化为树形结构。
模式缺陷：  设计复杂，结构不稳定或逻辑理解较难。

3.4 外观模式
面向对象：类。
常用场景：一个子系统需要对外提供服务
变化点：  无
选择关键点：  子系统对外提供服务是否需要依赖很多的类（降低了子系统与客户端之间的耦合度）。
模式缺陷：    增加新的子系统可能需要修改外观类或客户端的源代码，违背了“开闭原则”。
相关设计模式：中介者模式。
二者都是为了处理复杂的耦合关系，不同的是外观模式处理的是类之间复杂的依赖关系，中介者模式处理的是对象之间复杂的交互关系。

3.5 代理模式
面向对象：对象。
常用场景：需要修改或屏蔽某一个或若干个类的部分功能，复用另外一部分功能，可使用静态代理，
          若是需要拦截一批类中的某些方法，在方法的前后插入一些一致的操作，假设这些类有一致的接口，可使用JDK的动态代理，否则可使用cglib。
变化点：  静态代理没有变化点，动态代理的变化点为具有相同切入点的类。
选择关键点：静态代理选择的关键点是是否要复用被代理的部分功能，动态代理选择的关键点在于能否在将被代理的这一批类当中，找出相同的切入点
模式缺陷：切入点的不稳定。
相关设计模式：适配器模式，装饰器。
对于适配器模式当中的定制适配器，它与静态代理有着相似的部分，二者都有复用功能的作用，不同的是，静态代理会修改一部分原有的功能，
而适配器往往是全部复用，而且在复用的同时，适配器还会将复用的类适配一个接口。
装饰器模式和代理模式，都是包装，但一个是动态的给类添加职责，一个是控制对这个对象的访问。

3.6 享元模式
面向对象：对象。
常用场景：一些状态相同的对象被大量的重复使用。
变化点：  无。
选择关键点：被共享的对象是否可以将外部状态提取出来。
模式缺陷：  没有将外部状态提取完全。

3.7 桥接模式
面向对象：对象。
常用场景：一个对象有多个维度的变化，需要将这些维度抽离出来，让其独立变化。
变化点：  维度的扩展与增加。
选择关键点：是否可以将对象拆分成多个不相关的维度。
模式缺陷：  无。



创建型设计模式

4.1 单例模式
面向对象：无
常用场景：应用中有对象需要是全局的且唯一。
变化点：  无
选择关键点：一个对象在应用中出现多个实例是否会引起逻辑上或者是程序上的错误。
模式缺陷：  在以为是单例的情况下，却产生了多个实例。
相关设计模式：原型模式。
单例模式是只有一个实例，原型模式每拷贝一次都会创造一个新的实例。

4.2 简单工厂模式
面向对象：类。
常用场景：需要在一堆产品中选择其中一个产品。
变化点：  产品的种类。
选择关键点：一种产品是否可根据某个参数决定它的种类。
模式缺陷：  工厂类不能正常工作。
相关设计模式： 工厂方法。
工厂方法模式是简单工厂模式的进一步抽象化，在这两者之间做选择，主要看将工厂进一步抽象化是否有必要，
通常情况下，如果工厂的作用仅仅是用来制造产品，则没必要使用工厂方法模式。

4.3 工厂方法模式
面向对象：类。
常用场景：一种场景是希望工厂与产品的种类对客户端保持透明，给客户端提供一致的操作，另外一种是不同的工厂和产品可以提供客户端不同的服务或功能。
变化点：  工厂与产品的种类。
选择关键点：工厂类和产品类是否是同生同灭的关系。
模式缺陷：  子类增加较多。
相关设计模式： 抽象工厂模式。
工厂方法模式与抽象工厂模式最大的区别在于，在工厂方法模式中，工厂创造的是一个产品，而在抽象工厂模式中，工厂创造的是一个产品族。

4.4 抽象工厂模式
面向对象：类。
常用场景：需要一个接口可以提供一个产品族，且不必知道产品的具体种类。
变化点：  工厂与产品的种类。
选择关键点：产品族是否需要一起提供，且是否有一致的接口。
模式缺陷： 无。
相关设计模式：建造者模式。
两者都是建造一批对象或者说产品，不同的是两者的目的和实现手段，在建造者模式中，是为了复用对象的构建过程而定义了一个指挥者，
而在抽象工厂模式中，是为了提供一个这批对象的创建接口而定义了抽象工厂接口。

4.5 建造者模式
面向对象：类。
常用场景：需要构建一批构建过程相同但表示不同的产品，而构建过程非常复杂。
变化点：  产品的表示。
选择关键点：各个产品的构建过程是否相同。
模式缺陷：  指挥者不能正常工作。

4.6 原型模式
面向对象：类。
常用场景：需要在运行时动态的创建指定实例种类的对象，或是需要复用其状态。
变化点：  无。
选择关键点：创建出来的对象是否可以立即投入使用。
模式缺陷：  在以为是深度拷贝的情况下，却未实现深度拷贝。



行为型设计模式

5.1 模板方法模式
面向对象：类。
常用场景：一批子类的功能有可提取的公共算法骨架。
变化点：  算法骨架内各个步骤的具体实现。
选择关键点：算法骨架是否牢固。
模式缺陷：  无。

5.2 中介者模式
面向对象：对象。
常用场景：一个系列的对象交互关系十分复杂。
变化点：  对象之间的交互。
选择关键点：复杂的交互关系是否有共性可被中介者承担。
模式缺陷：  中介者无法工作。

5.3 策略模式
设计原则：对象。
常用场景：算法或者策略需要经常替换。
变化点：  策略的种类。
选择关键点：客户端是否依赖于某一个或若干个具体的策略。
模式缺陷：  无。

5.4 访问者模式
面向对象：对象。
常用场景：作用于一个数据结构之上的操作经常变化。
变化点：数据结构之上的操作。
选择关键点：数据结构是否稳定以及操作是否经常变化。
模式缺陷：数据结构的不稳定。

5.5 观察者模式
面向对象：对象。
常用场景：需要将观察者与被观察者解耦或者是观察者的种类不确定。
变化点：  观察者的种类与个数。
选择关键点：观察者与被观察者是否是多对一的关系。
模式缺陷：  观察者之间有过多的细节依赖。
相关设计模式： 中介者模式。
中介者模式是要抽象出多个对象之间的通信交互，观察者是抽象出一种一对多的单向通信。

5.6 状态模式
面向对象：对象。
常用场景：一个对象在多个状态下行为不同，且这些状态可互相转换。
变化点：  状态的种类。
选择关键点：这些状态是否经常在运行时需要在不同的动态之间相互转换。
模式缺陷：  状态模式的结构与实现都较为复杂。
相关设计模式：策略模式。
二者的实现方式非常相似，策略接口与状态接口，具体的策略与具体的状态以及二者都拥有的上下文，而二者不同的地方就在于，
状态模式经常会在处理请求的过程中更改上下文的状态，而策略模式只是按照不同的算法处理算法逻辑，
而且从实际场景来讲，顾名思义，状态模式改变的是状态，策略模式改变的是策略。

5.7 备忘录模式
面向对象：对象。
常用场景：需要在对象的外部保存该对象的内部状态。
变化点：  无。
选择关键点：是否可以在必要的时候捕捉到对象的内部状态。
模式缺陷：  大对象的备份。

5.8 职责链模式
面向对象：对象。
常用场景：一个请求的处理需要多个对象当中的一个或几个协作处理。
变化点：  处理链的长度与次序。
选择关键点：对于每一次请求是否每个处理的对象都需要一次处理机会。
模式缺陷：  无。

5.9 迭代器模式
面向对象：对象。
常用场景：需要迭代访问一个聚合对象中的各个元素，且不暴露该聚合对象内部的表示。
变化点：聚合对象的种类。
选择关键点：客户端是否关心遍历的次序。
模式缺陷：  无。
相关设计模式：访问者模式。
二者都是迭代的访问一个聚合对象中的各个元素，不同的是，访问者模式中，扩展开放的部分在作用于对象的操作上，
而迭代器模式中，扩展开放的部分在聚合对象的种类上，而且二者的实现方式也有着很大的区别。


