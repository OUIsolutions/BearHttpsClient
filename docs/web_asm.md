
## Step 1: dowload [BearHttpsClientOne.c](https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c)

```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c -o BearHttpsClientOne.c
```
## Step 2: create your main.c file

```c
#include "BearHttpsClientOne.c"


long set_text_area_value() {

    c2wasm_js_var c2wasm_input = c2wasm_call_object_prop(c2wasm_document, "getElementById", c2wasm_create_string("test_input"));
    c2wasm_js_var input_prop = c2wasm_get_object_prop_any(c2wasm_input, "value");
    int size =c2wasm_get_var_string_len(input_prop);
    char *url = malloc(sizeof(char) * size + 1);
    c2wasm_memcpy_string(input_prop,0,url,size);
    url[size] = '\0';
    BearHttpsRequest *request = newBearHttpsRequest(url);
    free(url);
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);


    if(BearHttpsResponse_error(response)){
        c2wasm_call_object_prop(c2wasm_window,"alert",c2wasm_create_string( BearHttpsResponse_get_error_msg(response)));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    printf("Headders:\n");
    for (int i = 0; i < BearHttpsResponse_get_headers_size(response); i++) {
        char *key = BearHttpsResponse_get_header_key_by_index(response, i);
        char *value = BearHttpsResponse_get_header_value_by_index(response, i);
    }

    const char *body = BearHttpsResponse_read_body_str(response);
    if(BearHttpsResponse_error(response)){
        c2wasm_call_object_prop(c2wasm_window,"alert",c2wasm_create_string( BearHttpsResponse_get_error_msg(response)));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    // Get the DOM element with id "test_div"
    c2wasm_js_var element = c2wasm_call_object_prop(c2wasm_document, "getElementById", c2wasm_create_string("test_div"));
    if(c2wasm_instance_of(element, c2wasm_error)) {
        printf("Error: Element not found\n");
        return c2wasm_undefined;
    }
    // Set the innerHTML of the element
    c2wasm_set_object_prop_string(element, "value", body);
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);

    return c2wasm_undefined;
}
int main(){
    c2wasm_start();
    // Expose the C function to JavaScript
    c2wasm_set_object_prop_function(c2wasm_window, "set_text_area_value", set_text_area_value);
    return 0;
}

```

### Step 3: compile with emcc

```bash
emcc main.c  -o main.js -sASYNCIFY
```
### Step 4: create index.html

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>C2Wasm Demo</title>
    <script src="main.js"></script>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/prism/1.29.0/themes/prism-tomorrow.min.css" rel="stylesheet">
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="container">
        <h1>🚀 C2Wasm Demo</h1>
        
        <div class="input-group">
            <label for="test_input">📝 Input de Comando:</label>
            <input type="text" id="test_input" placeholder="Digite seu comando aqui...">
        </div>

        <div class="editor-container">
            <div class="editor-header">
                <div class="editor-dots">
                    <div class="dot red"></div>
                    <div class="dot yellow"></div>
                    <div class="dot green"></div>
                </div>
                <div class="editor-title">Editor de Código</div>
            </div>
            <textarea id="test_div" placeholder="// Seu código aparecerá aqui...
// Use Ctrl+A para selecionar tudo
// Use Ctrl+C para copiar
// Use Tab para indentação

console.log('Hello World!');"></textarea>
        </div>

        <div class="button-container">
            <button onclick="set_text_area_value()">🔄 Atualizar Texto </button>
        </div>
    </div>
</body>
</html>

```
#### Step 5: create style.css

```css
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
            color: #333;
        }

        .container {
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            padding: 40px;
            backdrop-filter: blur(10px);
        }

        h1 {
            text-align: center;
            color: #2c3e50;
            font-size: 2.8rem;
            margin-bottom: 40px;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1);
            background: linear-gradient(45deg, #3498db, #9b59b6);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
        }

        .input-group {
            margin-bottom: 30px;
        }

        label {
            display: block;
            margin-bottom: 10px;
            font-weight: 600;
            color: #2c3e50;
            font-size: 1.1rem;
        }

        #test_input {
            width: 100%;
            padding: 15px 20px;
            border: 2px solid #e1e8ed;
            border-radius: 12px;
            font-size: 1rem;
            background: #fff;
            transition: all 0.3s ease;
            box-shadow: 0 2px 10px rgba(0, 0, 0, 0.05);
        }

        #test_input:focus {
            outline: none;
            border-color: #3498db;
            box-shadow: 0 0 20px rgba(52, 152, 219, 0.2);
            transform: translateY(-2px);
        }

        .editor-container {
            position: relative;
            margin-bottom: 30px;
        }

        #test_div {
            width: 100%;
            height: 500px;
            padding: 20px;
            border: 2px solid #e1e8ed;
            border-radius: 12px;
            font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
            font-size: 14px;
            line-height: 1.6;
            background: #1e1e1e;
            color: #d4d4d4;
            resize: vertical;
            min-height: 300px;
            transition: all 0.3s ease;
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
        }

        #test_div:focus {
            outline: none;
            border-color: #3498db;
            box-shadow: 0 0 25px rgba(52, 152, 219, 0.3);
        }

        .editor-header {
            background: #2c3e50;
            color: white;
            padding: 12px 20px;
            border-radius: 12px 12px 0 0;
            font-weight: 600;
            display: flex;
            align-items: center;
            gap: 10px;
        }

        .editor-dots {
            display: flex;
            gap: 6px;
        }

        .dot {
            width: 12px;
            height: 12px;
            border-radius: 50%;
        }

        .dot.red { background: #ff5f56; }
        .dot.yellow { background: #ffbd2e; }
        .dot.green { background: #27ca3f; }

        .editor-title {
            margin-left: auto;
            font-size: 0.9rem;
            opacity: 0.8;
        }

        .button-container {
            display: flex;
            gap: 15px;
            justify-content: center;
            flex-wrap: wrap;
        }

        button {
            background: linear-gradient(45deg, #3498db, #2980b9);
            color: white;
            border: none;
            padding: 15px 30px;
            border-radius: 50px;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 4px 15px rgba(52, 152, 219, 0.3);
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        button:hover {
            transform: translateY(-3px);
            box-shadow: 0 8px 25px rgba(52, 152, 219, 0.4);
            background: linear-gradient(45deg, #2980b9, #3498db);
        }

        button:active {
            transform: translateY(-1px);
        }

        .clear-btn {
            background: linear-gradient(45deg, #e74c3c, #c0392b);
            box-shadow: 0 4px 15px rgba(231, 76, 60, 0.3);
        }

        .clear-btn:hover {
            background: linear-gradient(45deg, #c0392b, #e74c3c);
            box-shadow: 0 8px 25px rgba(231, 76, 60, 0.4);
        }

        .copy-btn {
            background: linear-gradient(45deg, #27ae60, #2ecc71);
            box-shadow: 0 4px 15px rgba(39, 174, 96, 0.3);
        }

        .copy-btn:hover {
            background: linear-gradient(45deg, #2ecc71, #27ae60);
            box-shadow: 0 8px 25px rgba(39, 174, 96, 0.4);
        }

        @media (max-width: 768px) {
            .container {
                padding: 20px;
            }
            
            h1 {
                font-size: 2rem;
            }
            
            #test_div {
                height: 350px;
                font-size: 12px;
            }
            
            button {
                padding: 12px 24px;
                font-size: 0.9rem;
            }
        }

        .stats {
            margin-top: 20px;
            padding: 15px;
            background: rgba(52, 152, 219, 0.1);
            border-radius: 10px;
            display: flex;
            justify-content: space-around;
            flex-wrap: wrap;
            gap: 10px;
        }

        .stat-item {
            text-align: center;
        }

        .stat-number {
            font-size: 1.5rem;
            font-weight: bold;
            color: #3498db;
        }

        .stat-label {
            font-size: 0.9rem;
            color: #7f8c8d;
            margin-top: 5px;
        }
```

### Step 6: run the server

```bash
python3 -m http.server 8000
```
if you open your browser and try to fetch [example.com](https://example.com/), you will get an error like this:

```text
index.html:1 Access to fetch at 'https://example.com/' from origin 'http://localhost:8000' has been blocked by CORS policy: No 'Access-Control-Allow-Origin' header is present on the requested resource.
main.js:1791 
```
you can fix these by starting your chrome with the `--disable-web-security` flag:

```bash
google-chrome --disable-web-security --user-data-dir=/tmp/unsafe-chrome-profile index.html
```


