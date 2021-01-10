##Unity中的面向对象

> GameObject 类  --->游戏对象
> 所有的成员叫做组件：Component
> 每一个游戏对象都拥有Transform组件,包含3个属性(Postion -- 位置) ,(Rotation 选择 ),(Scale -- 缩放) 参考D3D11管线渲染过程
> Postion ： vector3对象（x,y,z）
> Rotation ：四元数（x,y,z,w）
> Scale ： Vector3（x,y,z）
> 定义一个集合：可以添加，移除，获取，自定义组件。基类是MonoBehaviour，内置了多个函数，按照顺序执行。
> 所有的脚本都继承MonoBehaviour可调用Unity现有的API。实际过程是多态。




