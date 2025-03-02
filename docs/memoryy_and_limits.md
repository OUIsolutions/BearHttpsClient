

#### Configurring Headder Read Properties
you can configure the headder read properties, using the following code:
```c
int read_size = 200;
double realocator_factor =1.5;
bear.request.set_chunk_headder_read_props(request,read_size,realocator_factor);
```
#### Configurring Body Read Properties

you can configure the body read properties, using the following code:
```c
int read_size = 200;
double realocator_factor =1.5;
 bear.response.set_body_read_props(response,read_size,realocator_factor);
```
#### Seting Body Limit
you can set the body  size limit (default its infinity), using the following code: 
```c
bear.response.set_max_body_size(response,1000);
```