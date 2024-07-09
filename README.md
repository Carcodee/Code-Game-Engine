# Code Game Engine
 
Simple 3d game
The main features are:


- PBR
- Imgui Graphic interface.
- ECS System
- Transform
- Content browser
- Drag and drop fbx files
- Async model loading.
- Bloom(not supported in the current version).
- Shadows(not supported in the current version).



To create your first code object you need to write in the "FirstCodeObject" file inside "EngineSandbox" the following code: 

 ```
void FirstCodeObject::StartCodeEngine()
{
	//is being callled on the parent class so the is being added also in the hirearchy of the parent class level
	FirstCodeObject* newObject=new FirstCodeObject(shader,modelHandlerController);
	this->AddParent(newObject);
}
```



You can also simply add the model from the content browser. After that you will see your model properties in the inspector.

Another Example to add an object with a model from a certain class:
 ```
void FirstCodeObject::StartCodeEngine()
{
	model= AddComponent<Model>();
	std::shared_ptr<Material> material = std::make_shared<Material>();
	model->StartModel("Assets/Models/box/Cube.obj",true, material);
	FirstCodeObject* newObject=new FirstCodeObject(shader,modelHandlerController);
	this->AddParent(newObject);
}

```



In this engine you can access to the components calling 
 ```
void FirstCodeObject::StartCodeEngine()
{
	model= GetComponent<T>();
}
```




Preview
![Preview](https://github.com/Carcodee/Code-Game-Engine/assets/74780908/680b5d4d-0f62-4be7-ac14-9f38a6d7aea7)
https://github.com/Carcodee/Code-Game-Engine/assets/74780908/1c33f831-1c53-47d0-91cc-d0c86e52334f


Set textures pasting the explorer filepath in the "Extract textures" of the model data, the file name with the textures should be the same as the one in the "Models/PBRTextures" folder.

![Muffing](https://github.com/Carcodee/Code-Game-Engine/assets/74780908/0944f88f-eb26-4359-ac55-f522bedc7313)
https://github.com/Carcodee/Code-Game-Engine/assets/74780908/ea3ecb3f-ef05-4571-a0a3-5a4b18159eb5

https://github.com/Carcodee/Code-Game-Engine/assets/74780908/306cddde-9b70-46fd-acb2-a0463540d003
![PBR](https://github.com/Carcodee/Code-Game-Engine/assets/74780908/a70d0330-c1f3-4309-87a8-4ba9975496bf)

Issues:

-Only supported for visual studio


PD: I would love to implement a lot more features but this is it for now. Any advice for this project is valuable :D

