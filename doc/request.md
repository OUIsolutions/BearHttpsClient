
## Configuring Headders
with the function **bear.request.add_headder** you can add a headder to the request. The function takes two arguments, the first is the headder name and the second is the headder value.

```c
bear.request.add_headder("Content-Type", "application/json");
```
## Configuring Methods 
with the function **bear.request.set_method** you can set the method of the request. The function takes one argument, the method name.

```c
bear.request.set_method("POST");
```
## Configuring URL
with the function **bear.request.set_url** you can set the URL of the request. The function takes one argument, the URL.

```c
bear.request.set_url("https://jsonplaceholder.typicode.com/posts");
```
## Configuring Body Upload