
# Complete Beginner's Guide to BearHttpsClient Web Assembly

Welcome! This guide will teach you how to create a web application that can make HTTP requests using C code that runs in the browser. Don't            <label for="tes            <button onclick="set_text_area_value()" class="fetch-btn">
                Fetch URL Content
            </button>
            <button onclick="clearTextArea()" class="clear-btn">
                Clear Results
            </button>
            <button onclick="copyContent()" class="copy-btn">
                Copy Content
            </button>Enter a URL to fetch:</label>worry if you're new to programming - we'll explain everything step by step!

## What You'll Build

By the end of this tutorial, you'll have:
- A web page with an input field and a text area
- The ability to enter a URL and fetch its content
- C code running directly in your browser (thanks to WebAssembly!)
- A beautiful, modern interface

## Prerequisites (What You Need First)

Before we start, make sure you have these installed on your computer:

### 1. **Emscripten** (Converts C code to WebAssembly)
```bash
# Download and install Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

### 2. **A Terminal/Command Line**
- On Linux: Use your default terminal
- On Mac: Use Terminal app
- On Windows: Use Command Prompt or PowerShell

### 3. **A Text Editor**
Any text editor works fine:
- VS Code (recommended for beginners)
- Sublime Text
- Notepad++
- Even basic Notepad works!

### 4. **Python** (For running a local web server)
Most systems have Python already installed. Test by running:
```bash
python3 --version
```

## Project Setup

First, let's create a folder for our project and organize it:

```bash
# Create a new folder for your project
mkdir my-web-app
cd my-web-app

# Create the files we'll need
touch main.c index.html style.css
```

## Step 1: Download the BearHttpsClient Library

The BearHttpsClient is a special library that lets C code make HTTP requests. We need to download it:

**Option A: Using curl (recommended)**
```bash
curl -L https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c -o BearHttpsClientOne.c
```

**Option B: Manual download**
1. Go to: https://github.com/OUIsolutions/BearHttpsClient/releases/download/0.5.0/BearHttpsClientOne.c
2. Right-click and "Save as..." → name it `BearHttpsClientOne.c`
3. Put it in your project folder
## Step 2: Create Your C Code (main.c)

Now we'll create the C code that will run in the browser. Don't worry if you don't understand every line - we'll explain the important parts!

**Create a file called `main.c` and copy this code:**

```c
#include "BearHttpsClientOne.c"

// This function will be called when the user clicks the button
long set_text_area_value() {

    // Step 1: Get the URL from the input field on the webpage
    c2wasm_js_var c2wasm_input = c2wasm_call_object_prop(c2wasm_document, "getElementById", c2wasm_create_string("test_input"));
    c2wasm_js_var input_prop = c2wasm_get_object_prop_any(c2wasm_input, "value");
    int size = c2wasm_get_var_string_len(input_prop);
    char *url = malloc(sizeof(char) * size + 1);
    c2wasm_memcpy_string(input_prop, 0, url, size);
    url[size] = '\0';
    
    // Step 2: Create an HTTP request
    BearHttpsRequest *request = newBearHttpsRequest(url);
    free(url);
    
    // Step 3: Send the request and get the response
    BearHttpsResponse *response = BearHttpsRequest_fetch(request);

    // Step 4: Check if there was an error
    if(BearHttpsResponse_error(response)){
        // Show an alert with the error message
        c2wasm_call_object_prop(c2wasm_window, "alert", c2wasm_create_string(BearHttpsResponse_get_error_msg(response)));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // Step 5: Optional - Print headers (you can see these in browser console)
    printf("Headers:\n");
    for (int i = 0; i < BearHttpsResponse_get_headers_size(response); i++) {
        char *key = BearHttpsResponse_get_header_key_by_index(response, i);
        char *value = BearHttpsResponse_get_header_value_by_index(response, i);
        printf("%s: %s\n", key, value);
    }

    // Step 6: Get the response body (the content of the webpage)
    const char *body = BearHttpsResponse_read_body_str(response);
    if(BearHttpsResponse_error(response)){
        c2wasm_call_object_prop(c2wasm_window, "alert", c2wasm_create_string(BearHttpsResponse_get_error_msg(response)));
        BearHttpsRequest_free(request);
        BearHttpsResponse_free(response);
        return 1;
    }
    
    // Step 7: Put the response into the text area on our webpage
    c2wasm_js_var element = c2wasm_call_object_prop(c2wasm_document, "getElementById", c2wasm_create_string("test_div"));
    if(c2wasm_instance_of(element, c2wasm_error)) {
        printf("Error: Element not found\n");
        return c2wasm_undefined;
    }
    
    // Set the content of the text area
    c2wasm_set_object_prop_string(element, "value", body);
    
    // Step 8: Clean up memory
    BearHttpsRequest_free(request);
    BearHttpsResponse_free(response);

    return c2wasm_undefined;
}

// This is the main function - it runs when the page loads
int main(){
    c2wasm_start();
    // Make our C function available to JavaScript
    c2wasm_set_object_prop_function(c2wasm_window, "set_text_area_value", set_text_area_value);
    return 0;
}
```

### What Does This Code Do?

1. **Gets user input**: Reads the URL from the input field
2. **Makes HTTP request**: Uses BearHttpsClient to fetch the webpage
3. **Handles errors**: Shows alerts if something goes wrong
4. **Displays result**: Puts the response in the text area
5. **Cleans up**: Frees memory to prevent leaks

## Step 3: Compile Your C Code to WebAssembly

This step converts your C code into WebAssembly so it can run in browsers:

**Run this command in your terminal:**

```bash
emcc main.c -o main.js -sASYNCIFY
```

**What happens here?**
- `emcc`: The Emscripten compiler
- `main.c`: Your C source file
- `-o main.js`: Output a JavaScript file called `main.js`
- `-sASYNCIFY`: Allows async operations (needed for HTTP requests)

**After running this, you'll see two new files:**
- `main.js`: The compiled WebAssembly + JavaScript glue code
- `main.wasm`: The actual WebAssembly binary

## Step 4: Create Your HTML Page (index.html)

This creates the visual interface for your app. **Create `index.html` with this content:**

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>My First Web Assembly App</title>
    <!-- Load our compiled C code -->
    <script src="main.js"></script>
    <!-- Load our custom styles -->
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="container">
        <h1>C-Powered Web Fetcher</h1>
        <p class="subtitle">Enter a URL below and watch C code fetch it in your browser!</p>
        
        <!-- Input field for the URL -->
        <div class="input-group">
            <label for="test_input">� Enter a URL to fetch:</label>
            <input type="text" 
                   id="test_input" 
                   placeholder="https://example.com or https://jsonplaceholder.typicode.com/posts/1"
                   value="https://jsonplaceholder.typicode.com/posts/1">
        </div>

        <!-- Text area to show the results -->
        <div class="editor-container">
            <div class="editor-header">
                <div class="editor-dots">
                    <div class="dot red"></div>
                    <div class="dot yellow"></div>
                    <div class="dot green"></div>
                </div>
                <div class="editor-title">Response Content</div>
            </div>
            <textarea id="test_div" 
                      placeholder="// Click the button below to fetch content!
// The response will appear here...
// You can scroll through the content and copy it

Ready to fetch some data?"></textarea>
        </div>

        <!-- Button to trigger the fetch -->
        <div class="button-container">
            <button onclick="set_text_area_value()" class="fetch-btn">
                � Fetch URL Content
            </button>
            <button onclick="clearTextArea()" class="clear-btn">
                🗑️ Clear Results
            </button>
            <button onclick="copyContent()" class="copy-btn">
                📋 Copy Content
            </button>
        </div>

        <!-- Help section -->
        <div class="help-section">
            <h3>Try These URLs:</h3>
            <ul>
                <li><code>https://jsonplaceholder.typicode.com/posts/1</code> - Sample JSON data</li>
                <li><code>https://httpbin.org/ip</code> - Your IP address</li>
                <li><code>https://api.github.com/users/octocat</code> - GitHub user info</li>
            </ul>
        </div>
    </div>

    <script>
        // Helper functions for the buttons
        function clearTextArea() {
            document.getElementById('test_div').value = '';
        }
        
        function copyContent() {
            const textarea = document.getElementById('test_div');
            textarea.select();
            document.execCommand('copy');
            alert('Content copied to clipboard!');
        }
        
        // Add some example URLs when clicked
        function setExampleURL(url) {
            document.getElementById('test_input').value = url;
        }
    </script>
</body>
</html>
```

### What's in This HTML?

- **Input field**: Where you type the URL to fetch
- **Text area**: Where the fetched content appears
- **Buttons**: To fetch, clear, and copy content
- **Helper functions**: JavaScript functions for better user experience
- **Example URLs**: Some safe URLs you can test with

## Step 5: Add Beautiful Styling (style.css)

This makes your app look professional and modern. **Create `style.css` with this content:**

```css
/* Reset and base styles */
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

/* Main container */
.container {
    max-width: 1200px;
    margin: 0 auto;
    background: rgba(255, 255, 255, 0.95);
    border-radius: 20px;
    box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
    padding: 40px;
    backdrop-filter: blur(10px);
}

/* Title styling */
h1 {
    text-align: center;
    color: #2c3e50;
    font-size: 2.8rem;
    margin-bottom: 10px;
    text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1);
    background: linear-gradient(45deg, #3498db, #9b59b6);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    background-clip: text;
}

.subtitle {
    text-align: center;
    color: #7f8c8d;
    font-size: 1.2rem;
    margin-bottom: 40px;
    font-weight: 300;
}

/* Input styling */
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

/* Editor/textarea styling */
.editor-container {
    position: relative;
    margin-bottom: 30px;
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

#test_div {
    width: 100%;
    height: 500px;
    padding: 20px;
    border: 2px solid #e1e8ed;
    border-top: none;
    border-radius: 0 0 12px 12px;
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

/* Button styling */
.button-container {
    display: flex;
    gap: 15px;
    justify-content: center;
    flex-wrap: wrap;
    margin-bottom: 30px;
}

button {
    border: none;
    padding: 15px 30px;
    border-radius: 50px;
    font-size: 1rem;
    font-weight: 600;
    cursor: pointer;
    transition: all 0.3s ease;
    text-transform: uppercase;
    letter-spacing: 1px;
}

.fetch-btn {
    background: linear-gradient(45deg, #3498db, #2980b9);
    color: white;
    box-shadow: 0 4px 15px rgba(52, 152, 219, 0.3);
}

.fetch-btn:hover {
    transform: translateY(-3px);
    box-shadow: 0 8px 25px rgba(52, 152, 219, 0.4);
    background: linear-gradient(45deg, #2980b9, #3498db);
}

.clear-btn {
    background: linear-gradient(45deg, #e74c3c, #c0392b);
    color: white;
    box-shadow: 0 4px 15px rgba(231, 76, 60, 0.3);
}

.clear-btn:hover {
    background: linear-gradient(45deg, #c0392b, #e74c3c);
    box-shadow: 0 8px 25px rgba(231, 76, 60, 0.4);
    transform: translateY(-3px);
}

.copy-btn {
    background: linear-gradient(45deg, #27ae60, #2ecc71);
    color: white;
    box-shadow: 0 4px 15px rgba(39, 174, 96, 0.3);
}

.copy-btn:hover {
    background: linear-gradient(45deg, #2ecc71, #27ae60);
    box-shadow: 0 8px 25px rgba(39, 174, 96, 0.4);
    transform: translateY(-3px);
}

button:active {
    transform: translateY(-1px);
}

/* Help section */
.help-section {
    background: rgba(52, 152, 219, 0.1);
    padding: 25px;
    border-radius: 15px;
    border-left: 5px solid #3498db;
}

.help-section h3 {
    color: #2c3e50;
    margin-bottom: 15px;
    font-size: 1.3rem;
}

.help-section ul {
    list-style: none;
    padding: 0;
}

.help-section li {
    margin-bottom: 10px;
    padding: 10px;
    background: rgba(255, 255, 255, 0.7);
    border-radius: 8px;
    transition: all 0.3s ease;
}

.help-section li:hover {
    background: rgba(255, 255, 255, 0.9);
    transform: translateX(5px);
}

.help-section code {
    background: #2c3e50;
    color: #ecf0f1;
    padding: 4px 8px;
    border-radius: 4px;
    font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
    font-size: 0.9rem;
}

/* Responsive design */
@media (max-width: 768px) {
    .container {
        padding: 20px;
    }
    
    h1 {
        font-size: 2rem;
    }
    
    .subtitle {
        font-size: 1rem;
    }
    
    #test_div {
        height: 350px;
        font-size: 12px;
    }
    
    button {
        padding: 12px 24px;
        font-size: 0.9rem;
    }
    
    .help-section {
        padding: 15px;
    }
}
```

## Step 6: Start Your Web Server

Your app needs to run on a web server (not just open the HTML file directly). Here's why and how:

**Why do we need a web server?**
- WebAssembly files must be loaded over HTTP (not file://)
- Security restrictions prevent direct file access
- A local server solves this easily

**Start the server:**
```bash
# Navigate to your project folder (if not already there)
cd my-web-app

# Start a simple Python web server
python3 -m http.server 8000
```

**What you'll see:**
```
Serving HTTP on 0.0.0.0 port 8000 (http://0.0.0.0:8000/) ...
```

**Open your browser and visit:**
```
http://localhost:8000
```

**Congratulations!** Your app should now be running!

## Solving CORS Issues

When you try to fetch from external websites (like `https://example.com/`), you might see this error:

```
Access to fetch at 'https://example.com/' from origin 'http://localhost:8000' 
has been blocked by CORS policy: No 'Access-Control-Allow-Origin' header is present...
```

**What is CORS?**
CORS (Cross-Origin Resource Sharing) is a security feature that prevents websites from making requests to other domains without permission.

**Solution: Disable web security for testing**

**IMPORTANT:** Only do this for development/testing, never for regular browsing!

**For Chrome/Chromium:**
```bash
# Close all Chrome windows first, then run:
google-chrome --disable-web-security --user-data-dir=/tmp/unsafe-chrome-profile http://localhost:8000
```

**For other browsers:**
- **Firefox**: You can install the "CORS Everywhere" extension for testing
- **Safari**: Enable "Develop" menu, then "Disable Cross-Origin Restrictions"

## Testing Your App

### Step 1: Start with safe URLs
Try these URLs that allow CORS:
- `https://jsonplaceholder.typicode.com/posts/1`
- `https://httpbin.org/ip`
- `https://api.github.com/users/octocat`

### Step 2: Watch the magic happen!
1. Enter a URL in the input field
2. Click "Fetch URL Content"
3. Watch as your C code runs in the browser and fetches the content!

### Step 3: Try different content types
- JSON APIs return structured data
- Regular websites return HTML
- Some APIs return plain text

## Troubleshooting Guide

### Problem: "Module not found" error
**Solution:** Make sure all files are in the same folder:
```
my-web-app/
├── BearHttpsClientOne.c
├── main.c
├── main.js (created by emcc)
├── main.wasm (created by emcc)
├── index.html
└── style.css
```

### Problem: Compilation fails
**Solution:** Check that:
1. Emscripten is properly installed and activated
2. `BearHttpsClientOne.c` is in the same folder as `main.c`
3. You're running the command from the right directory

### Problem: Nothing happens when clicking the button
**Solution:**
1. Open browser developer tools (F12)
2. Check the Console tab for error messages
3. Make sure the server is still running

### Problem: CORS errors
**Solution:**
1. Use the browser flags mentioned above
2. Try the suggested "CORS-friendly" URLs first
3. Remember this is just for development

## Next Steps & Ideas

### Enhance Your App
1. **Add a loading spinner** while fetching
2. **Format JSON responses** with proper indentation
3. **Add HTTP headers display** in a separate section
4. **Support different HTTP methods** (POST, PUT, DELETE)
5. **Add request history** to remember previous requests

### Learn More
1. **Study the C code** - try to understand each line
2. **Experiment with different URLs** and see how responses vary
3. **Read about WebAssembly** - how does it work?
4. **Explore the BearHttpsClient** - what other features does it have?

### Share Your Creation
1. **Deploy to GitHub Pages** for free hosting
2. **Show friends** how C code can run in browsers
3. **Modify the styling** to match your preferences
4. **Add your own features** and make it unique

## What You've Learned

- **WebAssembly basics** - How to run C code in browsers  
- **Emscripten compilation** - Converting C to WebAssembly  
- **HTTP requests in C** - Using BearHttpsClient library  
- **Web development** - HTML, CSS, and JavaScript integration  
- **CORS understanding** - Browser security and how to work with it  
- **Local development** - Setting up and running a web server  

## Additional Resources

- **Emscripten Documentation**: https://emscripten.org/docs/
- **WebAssembly Official Site**: https://webassembly.org/
- **BearHttpsClient GitHub**: https://github.com/OUIsolutions/BearHttpsClient
- **MDN Web Docs (CORS)**: https://developer.mozilla.org/en-US/docs/Web/HTTP/CORS

---

**Congratulations on building your first WebAssembly application!** 

You've successfully created a web app that runs C code in the browser, makes HTTP requests, and displays the results with a beautiful interface. This is just the beginning of what you can do with WebAssembly and C!

Keep experimenting, learning, and building amazing things!


