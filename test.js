// include: shell.js
// The Module object: Our interface to the outside world. We import
// and export values on it. There are various ways Module can be used:
// 1. Not defined. We create it here
// 2. A function parameter, function(moduleArg) => Promise<Module>
// 3. pre-run appended it, var Module = {}; ..generated code..
// 4. External script tag defines var Module.
// We need to check if Module already exists (e.g. case 3 above).
// Substitution will be replaced with actual code on later stage of the build,
// this way Closure Compiler will not mangle it (e.g. case 4. above).
// Note that if you want to run closure, and also to use Module
// after the generated code, you will need to define   var Module = {};
// before the code. Then that object will be used in the code, and you
// can continue to use Module afterwards as well.
var Module = typeof Module != 'undefined' ? Module : {};

// Determine the runtime environment we are in. You can customize this by
// setting the ENVIRONMENT setting at compile time (see settings.js).

// Attempt to auto-detect the environment
var ENVIRONMENT_IS_WEB = typeof window == 'object';
var ENVIRONMENT_IS_WORKER = typeof WorkerGlobalScope != 'undefined';
// N.b. Electron.js environment is simultaneously a NODE-environment, but
// also a web environment.
var ENVIRONMENT_IS_NODE = typeof process == 'object' && typeof process.versions == 'object' && typeof process.versions.node == 'string' && process.type != 'renderer';
var ENVIRONMENT_IS_SHELL = !ENVIRONMENT_IS_WEB && !ENVIRONMENT_IS_NODE && !ENVIRONMENT_IS_WORKER;

if (ENVIRONMENT_IS_NODE) {

}

// --pre-jses are emitted after the Module integration code, so that they can
// refer to Module (if they choose; they can also define Module)


var arguments_ = [];
var thisProgram = './this.program';
var quit_ = (status, toThrow) => {
  throw toThrow;
};

// `/` should be present at the end if `scriptDirectory` is not empty
var scriptDirectory = '';
function locateFile(path) {
  if (Module['locateFile']) {
    return Module['locateFile'](path, scriptDirectory);
  }
  return scriptDirectory + path;
}

// Hooks that are implemented differently in different runtime environments.
var readAsync, readBinary;

if (ENVIRONMENT_IS_NODE) {
  if (typeof process == 'undefined' || !process.release || process.release.name !== 'node') throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

  var nodeVersion = process.versions.node;
  var numericVersion = nodeVersion.split('.').slice(0, 3);
  numericVersion = (numericVersion[0] * 10000) + (numericVersion[1] * 100) + (numericVersion[2].split('-')[0] * 1);
  var minVersion = 160000;
  if (numericVersion < 160000) {
    throw new Error('This emscripten-generated code requires node v16.0.0 (detected v' + nodeVersion + ')');
  }

  // These modules will usually be used on Node.js. Load them eagerly to avoid
  // the complexity of lazy-loading.
  var fs = require('fs');
  var nodePath = require('path');

  scriptDirectory = __dirname + '/';

// include: node_shell_read.js
readBinary = (filename) => {
  // We need to re-wrap `file://` strings to URLs.
  filename = isFileURI(filename) ? new URL(filename) : filename;
  var ret = fs.readFileSync(filename);
  assert(Buffer.isBuffer(ret));
  return ret;
};

readAsync = async (filename, binary = true) => {
  // See the comment in the `readBinary` function.
  filename = isFileURI(filename) ? new URL(filename) : filename;
  var ret = fs.readFileSync(filename, binary ? undefined : 'utf8');
  assert(binary ? Buffer.isBuffer(ret) : typeof ret == 'string');
  return ret;
};
// end include: node_shell_read.js
  if (process.argv.length > 1) {
    thisProgram = process.argv[1].replace(/\\/g, '/');
  }

  arguments_ = process.argv.slice(2);

  // MODULARIZE will export the module in the proper place outside, we don't need to export here
  if (typeof module != 'undefined') {
    module['exports'] = Module;
  }

  quit_ = (status, toThrow) => {
    process.exitCode = status;
    throw toThrow;
  };

} else
if (ENVIRONMENT_IS_SHELL) {

  if ((typeof process == 'object' && typeof require === 'function') || typeof window == 'object' || typeof WorkerGlobalScope != 'undefined') throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

} else

// Note that this includes Node.js workers when relevant (pthreads is enabled).
// Node.js workers are detected as a combination of ENVIRONMENT_IS_WORKER and
// ENVIRONMENT_IS_NODE.
if (ENVIRONMENT_IS_WEB || ENVIRONMENT_IS_WORKER) {
  if (ENVIRONMENT_IS_WORKER) { // Check worker, not web, since window could be polyfilled
    scriptDirectory = self.location.href;
  } else if (typeof document != 'undefined' && document.currentScript) { // web
    scriptDirectory = document.currentScript.src;
  }
  // blob urls look like blob:http://site.com/etc/etc and we cannot infer anything from them.
  // otherwise, slice off the final part of the url to find the script directory.
  // if scriptDirectory does not contain a slash, lastIndexOf will return -1,
  // and scriptDirectory will correctly be replaced with an empty string.
  // If scriptDirectory contains a query (starting with ?) or a fragment (starting with #),
  // they are removed because they could contain a slash.
  if (scriptDirectory.startsWith('blob:')) {
    scriptDirectory = '';
  } else {
    scriptDirectory = scriptDirectory.slice(0, scriptDirectory.replace(/[?#].*/, '').lastIndexOf('/')+1);
  }

  if (!(typeof window == 'object' || typeof WorkerGlobalScope != 'undefined')) throw new Error('not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)');

  {
// include: web_or_worker_shell_read.js
if (ENVIRONMENT_IS_WORKER) {
    readBinary = (url) => {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', url, false);
      xhr.responseType = 'arraybuffer';
      xhr.send(null);
      return new Uint8Array(/** @type{!ArrayBuffer} */(xhr.response));
    };
  }

  readAsync = async (url) => {
    // Fetch has some additional restrictions over XHR, like it can't be used on a file:// url.
    // See https://github.com/github/fetch/pull/92#issuecomment-140665932
    // Cordova or Electron apps are typically loaded from a file:// url.
    // So use XHR on webview if URL is a file URL.
    if (isFileURI(url)) {
      return new Promise((resolve, reject) => {
        var xhr = new XMLHttpRequest();
        xhr.open('GET', url, true);
        xhr.responseType = 'arraybuffer';
        xhr.onload = () => {
          if (xhr.status == 200 || (xhr.status == 0 && xhr.response)) { // file URLs can return 0
            resolve(xhr.response);
            return;
          }
          reject(xhr.status);
        };
        xhr.onerror = reject;
        xhr.send(null);
      });
    }
    var response = await fetch(url, { credentials: 'same-origin' });
    if (response.ok) {
      return response.arrayBuffer();
    }
    throw new Error(response.status + ' : ' + response.url);
  };
// end include: web_or_worker_shell_read.js
  }
} else
{
  throw new Error('environment detection error');
}

var out = console.log.bind(console);
var err = console.error.bind(console);

var IDBFS = 'IDBFS is no longer included by default; build with -lidbfs.js';
var PROXYFS = 'PROXYFS is no longer included by default; build with -lproxyfs.js';
var WORKERFS = 'WORKERFS is no longer included by default; build with -lworkerfs.js';
var FETCHFS = 'FETCHFS is no longer included by default; build with -lfetchfs.js';
var ICASEFS = 'ICASEFS is no longer included by default; build with -licasefs.js';
var JSFILEFS = 'JSFILEFS is no longer included by default; build with -ljsfilefs.js';
var OPFS = 'OPFS is no longer included by default; build with -lopfs.js';

var NODEFS = 'NODEFS is no longer included by default; build with -lnodefs.js';

// perform assertions in shell.js after we set up out() and err(), as otherwise
// if an assertion fails it cannot print the message

assert(!ENVIRONMENT_IS_SHELL, 'shell environment detected but not enabled at build time.  Add `shell` to `-sENVIRONMENT` to enable.');

// end include: shell.js

// include: preamble.js
// === Preamble library stuff ===

// Documentation for the public APIs defined in this file must be updated in:
//    site/source/docs/api_reference/preamble.js.rst
// A prebuilt local version of the documentation is available at:
//    site/build/text/docs/api_reference/preamble.js.txt
// You can also build docs locally as HTML or other formats in site/
// An online HTML version (which may be of a different version of Emscripten)
//    is up at http://kripken.github.io/emscripten-site/docs/api_reference/preamble.js.html

var wasmBinary;

if (typeof WebAssembly != 'object') {
  err('no native wasm support detected');
}

// Wasm globals

var wasmMemory;

//========================================
// Runtime essentials
//========================================

// whether we are quitting the application. no code should run after this.
// set in exit() and abort()
var ABORT = false;

// set by exit() and abort().  Passed to 'onExit' handler.
// NOTE: This is also used as the process return code code in shell environments
// but only when noExitRuntime is false.
var EXITSTATUS;

// In STRICT mode, we only define assert() when ASSERTIONS is set.  i.e. we
// don't define it at all in release modes.  This matches the behaviour of
// MINIMAL_RUNTIME.
// TODO(sbc): Make this the default even without STRICT enabled.
/** @type {function(*, string=)} */
function assert(condition, text) {
  if (!condition) {
    abort('Assertion failed' + (text ? ': ' + text : ''));
  }
}

// We used to include malloc/free by default in the past. Show a helpful error in
// builds with assertions.

// Memory management

var HEAP,
/** @type {!Int8Array} */
  HEAP8,
/** @type {!Uint8Array} */
  HEAPU8,
/** @type {!Int16Array} */
  HEAP16,
/** @type {!Uint16Array} */
  HEAPU16,
/** @type {!Int32Array} */
  HEAP32,
/** @type {!Uint32Array} */
  HEAPU32,
/** @type {!Float32Array} */
  HEAPF32,
/* BigInt64Array type is not correctly defined in closure
/** not-@type {!BigInt64Array} */
  HEAP64,
/* BigUint64Array type is not correctly defined in closure
/** not-t@type {!BigUint64Array} */
  HEAPU64,
/** @type {!Float64Array} */
  HEAPF64;

var runtimeInitialized = false;

/**
 * Indicates whether filename is delivered via file protocol (as opposed to http/https)
 * @noinline
 */
var isFileURI = (filename) => filename.startsWith('file://');

// include: runtime_shared.js
// include: runtime_stack_check.js
// Initializes the stack cookie. Called at the startup of main and at the startup of each thread in pthreads mode.
function writeStackCookie() {
  var max = _emscripten_stack_get_end();
  assert((max & 3) == 0);
  // If the stack ends at address zero we write our cookies 4 bytes into the
  // stack.  This prevents interference with SAFE_HEAP and ASAN which also
  // monitor writes to address zero.
  if (max == 0) {
    max += 4;
  }
  // The stack grow downwards towards _emscripten_stack_get_end.
  // We write cookies to the final two words in the stack and detect if they are
  // ever overwritten.
  HEAPU32[((max)>>2)] = 0x02135467;
  HEAPU32[(((max)+(4))>>2)] = 0x89BACDFE;
  // Also test the global address 0 for integrity.
  HEAPU32[((0)>>2)] = 1668509029;
}

function checkStackCookie() {
  if (ABORT) return;
  var max = _emscripten_stack_get_end();
  // See writeStackCookie().
  if (max == 0) {
    max += 4;
  }
  var cookie1 = HEAPU32[((max)>>2)];
  var cookie2 = HEAPU32[(((max)+(4))>>2)];
  if (cookie1 != 0x02135467 || cookie2 != 0x89BACDFE) {
    abort(`Stack overflow! Stack cookie has been overwritten at ${ptrToString(max)}, expected hex dwords 0x89BACDFE and 0x2135467, but received ${ptrToString(cookie2)} ${ptrToString(cookie1)}`);
  }
  // Also test the global address 0 for integrity.
  if (HEAPU32[((0)>>2)] != 0x63736d65 /* 'emsc' */) {
    abort('Runtime error: The application has corrupted its heap memory area (address zero)!');
  }
}
// end include: runtime_stack_check.js
// include: runtime_exceptions.js
// end include: runtime_exceptions.js
// include: runtime_debug.js
// Endianness check
(() => {
  var h16 = new Int16Array(1);
  var h8 = new Int8Array(h16.buffer);
  h16[0] = 0x6373;
  if (h8[0] !== 0x73 || h8[1] !== 0x63) throw 'Runtime error: expected the system to be little-endian! (Run with -sSUPPORT_BIG_ENDIAN to bypass)';
})();

function consumedModuleProp(prop) {
  if (!Object.getOwnPropertyDescriptor(Module, prop)) {
    Object.defineProperty(Module, prop, {
      configurable: true,
      set() {
        abort(`Attempt to set \`Module.${prop}\` after it has already been processed.  This can happen, for example, when code is injected via '--post-js' rather than '--pre-js'`);

      }
    });
  }
}

function ignoredModuleProp(prop) {
  if (Object.getOwnPropertyDescriptor(Module, prop)) {
    abort(`\`Module.${prop}\` was supplied but \`${prop}\` not included in INCOMING_MODULE_JS_API`);
  }
}

// forcing the filesystem exports a few things by default
function isExportedByForceFilesystem(name) {
  return name === 'FS_createPath' ||
         name === 'FS_createDataFile' ||
         name === 'FS_createPreloadedFile' ||
         name === 'FS_unlink' ||
         name === 'addRunDependency' ||
         // The old FS has some functionality that WasmFS lacks.
         name === 'FS_createLazyFile' ||
         name === 'FS_createDevice' ||
         name === 'removeRunDependency';
}

/**
 * Intercept access to a global symbol.  This enables us to give informative
 * warnings/errors when folks attempt to use symbols they did not include in
 * their build, or no symbols that no longer exist.
 */
function hookGlobalSymbolAccess(sym, func) {
  if (typeof globalThis != 'undefined' && !Object.getOwnPropertyDescriptor(globalThis, sym)) {
    Object.defineProperty(globalThis, sym, {
      configurable: true,
      get() {
        func();
        return undefined;
      }
    });
  }
}

function missingGlobal(sym, msg) {
  hookGlobalSymbolAccess(sym, () => {
    warnOnce(`\`${sym}\` is not longer defined by emscripten. ${msg}`);
  });
}

missingGlobal('buffer', 'Please use HEAP8.buffer or wasmMemory.buffer');
missingGlobal('asm', 'Please use wasmExports instead');

function missingLibrarySymbol(sym) {
  hookGlobalSymbolAccess(sym, () => {
    // Can't `abort()` here because it would break code that does runtime
    // checks.  e.g. `if (typeof SDL === 'undefined')`.
    var msg = `\`${sym}\` is a library symbol and not included by default; add it to your library.js __deps or to DEFAULT_LIBRARY_FUNCS_TO_INCLUDE on the command line`;
    // DEFAULT_LIBRARY_FUNCS_TO_INCLUDE requires the name as it appears in
    // library.js, which means $name for a JS name with no prefix, or name
    // for a JS name like _name.
    var librarySymbol = sym;
    if (!librarySymbol.startsWith('_')) {
      librarySymbol = '$' + sym;
    }
    msg += ` (e.g. -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='${librarySymbol}')`;
    if (isExportedByForceFilesystem(sym)) {
      msg += '. Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you';
    }
    warnOnce(msg);
  });

  // Any symbol that is not included from the JS library is also (by definition)
  // not exported on the Module object.
  unexportedRuntimeSymbol(sym);
}

function unexportedRuntimeSymbol(sym) {
  if (!Object.getOwnPropertyDescriptor(Module, sym)) {
    Object.defineProperty(Module, sym, {
      configurable: true,
      get() {
        var msg = `'${sym}' was not exported. add it to EXPORTED_RUNTIME_METHODS (see the Emscripten FAQ)`;
        if (isExportedByForceFilesystem(sym)) {
          msg += '. Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you';
        }
        abort(msg);
      }
    });
  }
}

var runtimeDebug = true; // Switch to false at runtime to disable logging at the right times

// Used by XXXXX_DEBUG settings to output debug messages.
function dbg(...args) {
  if (!runtimeDebug && typeof runtimeDebug != 'undefined') return;
  // TODO(sbc): Make this configurable somehow.  Its not always convenient for
  // logging to show up as warnings.
  console.warn(...args);
}
// end include: runtime_debug.js
// include: memoryprofiler.js
// end include: memoryprofiler.js


function updateMemoryViews() {
  var b = wasmMemory.buffer;
  HEAP8 = new Int8Array(b);
  HEAP16 = new Int16Array(b);
  HEAPU8 = new Uint8Array(b);
  HEAPU16 = new Uint16Array(b);
  HEAP32 = new Int32Array(b);
  HEAPU32 = new Uint32Array(b);
  HEAPF32 = new Float32Array(b);
  HEAPF64 = new Float64Array(b);
  HEAP64 = new BigInt64Array(b);
  HEAPU64 = new BigUint64Array(b);
}

// end include: runtime_shared.js
assert(typeof Int32Array != 'undefined' && typeof Float64Array !== 'undefined' && Int32Array.prototype.subarray != undefined && Int32Array.prototype.set != undefined,
       'JS engine does not provide full typed array support');

function preRun() {
  if (Module['preRun']) {
    if (typeof Module['preRun'] == 'function') Module['preRun'] = [Module['preRun']];
    while (Module['preRun'].length) {
      addOnPreRun(Module['preRun'].shift());
    }
  }
  consumedModuleProp('preRun');
  // Begin ATPRERUNS hooks
  callRuntimeCallbacks(onPreRuns);
  // End ATPRERUNS hooks
}

function initRuntime() {
  assert(!runtimeInitialized);
  runtimeInitialized = true;

  checkStackCookie();

  // No ATINITS hooks

  wasmExports['__wasm_call_ctors']();

  // No ATPOSTCTORS hooks
}

function preMain() {
  checkStackCookie();
  // No ATMAINS hooks
}

function postRun() {
  checkStackCookie();

  if (Module['postRun']) {
    if (typeof Module['postRun'] == 'function') Module['postRun'] = [Module['postRun']];
    while (Module['postRun'].length) {
      addOnPostRun(Module['postRun'].shift());
    }
  }
  consumedModuleProp('postRun');

  // Begin ATPOSTRUNS hooks
  callRuntimeCallbacks(onPostRuns);
  // End ATPOSTRUNS hooks
}

// A counter of dependencies for calling run(). If we need to
// do asynchronous work before running, increment this and
// decrement it. Incrementing must happen in a place like
// Module.preRun (used by emcc to add file preloading).
// Note that you can add dependencies in preRun, even though
// it happens right before run - run will be postponed until
// the dependencies are met.
var runDependencies = 0;
var dependenciesFulfilled = null; // overridden to take different actions when all run dependencies are fulfilled
var runDependencyTracking = {};
var runDependencyWatcher = null;

function getUniqueRunDependency(id) {
  var orig = id;
  while (1) {
    if (!runDependencyTracking[id]) return id;
    id = orig + Math.random();
  }
}

function addRunDependency(id) {
  runDependencies++;

  Module['monitorRunDependencies']?.(runDependencies);

  if (id) {
    assert(!runDependencyTracking[id]);
    runDependencyTracking[id] = 1;
    if (runDependencyWatcher === null && typeof setInterval != 'undefined') {
      // Check for missing dependencies every few seconds
      runDependencyWatcher = setInterval(() => {
        if (ABORT) {
          clearInterval(runDependencyWatcher);
          runDependencyWatcher = null;
          return;
        }
        var shown = false;
        for (var dep in runDependencyTracking) {
          if (!shown) {
            shown = true;
            err('still waiting on run dependencies:');
          }
          err(`dependency: ${dep}`);
        }
        if (shown) {
          err('(end of list)');
        }
      }, 10000);
    }
  } else {
    err('warning: run dependency added without ID');
  }
}

function removeRunDependency(id) {
  runDependencies--;

  Module['monitorRunDependencies']?.(runDependencies);

  if (id) {
    assert(runDependencyTracking[id]);
    delete runDependencyTracking[id];
  } else {
    err('warning: run dependency removed without ID');
  }
  if (runDependencies == 0) {
    if (runDependencyWatcher !== null) {
      clearInterval(runDependencyWatcher);
      runDependencyWatcher = null;
    }
    if (dependenciesFulfilled) {
      var callback = dependenciesFulfilled;
      dependenciesFulfilled = null;
      callback(); // can add another dependenciesFulfilled
    }
  }
}

/** @param {string|number=} what */
function abort(what) {
  Module['onAbort']?.(what);

  what = 'Aborted(' + what + ')';
  // TODO(sbc): Should we remove printing and leave it up to whoever
  // catches the exception?
  err(what);

  ABORT = true;

  if (what.indexOf('RuntimeError: unreachable') >= 0) {
    what += '. "unreachable" may be due to ASYNCIFY_STACK_SIZE not being large enough (try increasing it)';
  }

  // Use a wasm runtime error, because a JS error might be seen as a foreign
  // exception, which means we'd run destructors on it. We need the error to
  // simply make the program stop.
  // FIXME This approach does not work in Wasm EH because it currently does not assume
  // all RuntimeErrors are from traps; it decides whether a RuntimeError is from
  // a trap or not based on a hidden field within the object. So at the moment
  // we don't have a way of throwing a wasm trap from JS. TODO Make a JS API that
  // allows this in the wasm spec.

  // Suppress closure compiler warning here. Closure compiler's builtin extern
  // definition for WebAssembly.RuntimeError claims it takes no arguments even
  // though it can.
  // TODO(https://github.com/google/closure-compiler/pull/3913): Remove if/when upstream closure gets fixed.
  /** @suppress {checkTypes} */
  var e = new WebAssembly.RuntimeError(what);

  // Throw the error whether or not MODULARIZE is set because abort is used
  // in code paths apart from instantiation where an exception is expected
  // to be thrown when abort is called.
  throw e;
}

// show errors on likely calls to FS when it was not included
var FS = {
  error() {
    abort('Filesystem support (FS) was not included. The problem is that you are using files from JS, but files were not used from C/C++, so filesystem support was not auto-included. You can force-include filesystem support with -sFORCE_FILESYSTEM');
  },
  init() { FS.error() },
  createDataFile() { FS.error() },
  createPreloadedFile() { FS.error() },
  createLazyFile() { FS.error() },
  open() { FS.error() },
  mkdev() { FS.error() },
  registerDevice() { FS.error() },
  analyzePath() { FS.error() },

  ErrnoError() { FS.error() },
};


function createExportWrapper(name, nargs) {
  return (...args) => {
    assert(runtimeInitialized, `native function \`${name}\` called before runtime initialization`);
    var f = wasmExports[name];
    assert(f, `exported native function \`${name}\` not found`);
    // Only assert for too many arguments. Too few can be valid since the missing arguments will be zero filled.
    assert(args.length <= nargs, `native function \`${name}\` called with ${args.length} args but expects ${nargs}`);
    return f(...args);
  };
}

var wasmBinaryFile;

function findWasmBinary() {
    return locateFile('test.wasm');
}

function getBinarySync(file) {
  if (file == wasmBinaryFile && wasmBinary) {
    return new Uint8Array(wasmBinary);
  }
  if (readBinary) {
    return readBinary(file);
  }
  throw 'both async and sync fetching of the wasm failed';
}

async function getWasmBinary(binaryFile) {
  // If we don't have the binary yet, load it asynchronously using readAsync.
  if (!wasmBinary) {
    // Fetch the binary using readAsync
    try {
      var response = await readAsync(binaryFile);
      return new Uint8Array(response);
    } catch {
      // Fall back to getBinarySync below;
    }
  }

  // Otherwise, getBinarySync should be able to get it synchronously
  return getBinarySync(binaryFile);
}

async function instantiateArrayBuffer(binaryFile, imports) {
  try {
    var binary = await getWasmBinary(binaryFile);
    var instance = await WebAssembly.instantiate(binary, imports);
    return instance;
  } catch (reason) {
    err(`failed to asynchronously prepare wasm: ${reason}`);

    // Warn on some common problems.
    if (isFileURI(wasmBinaryFile)) {
      err(`warning: Loading from a file URI (${wasmBinaryFile}) is not supported in most browsers. See https://emscripten.org/docs/getting_started/FAQ.html#how-do-i-run-a-local-webserver-for-testing-why-does-my-program-stall-in-downloading-or-preparing`);
    }
    abort(reason);
  }
}

async function instantiateAsync(binary, binaryFile, imports) {
  if (!binary && typeof WebAssembly.instantiateStreaming == 'function'
      // Don't use streaming for file:// delivered objects in a webview, fetch them synchronously.
      && !isFileURI(binaryFile)
      // Avoid instantiateStreaming() on Node.js environment for now, as while
      // Node.js v18.1.0 implements it, it does not have a full fetch()
      // implementation yet.
      //
      // Reference:
      //   https://github.com/emscripten-core/emscripten/pull/16917
      && !ENVIRONMENT_IS_NODE
     ) {
    try {
      var response = fetch(binaryFile, { credentials: 'same-origin' });
      var instantiationResult = await WebAssembly.instantiateStreaming(response, imports);
      return instantiationResult;
    } catch (reason) {
      // We expect the most common failure cause to be a bad MIME type for the binary,
      // in which case falling back to ArrayBuffer instantiation should work.
      err(`wasm streaming compile failed: ${reason}`);
      err('falling back to ArrayBuffer instantiation');
      // fall back of instantiateArrayBuffer below
    };
  }
  return instantiateArrayBuffer(binaryFile, imports);
}

function getWasmImports() {
  // instrumenting imports is used in asyncify in two ways: to add assertions
  // that check for proper import use, and for ASYNCIFY=2 we use them to set up
  // the Promise API on the import side.
  Asyncify.instrumentWasmImports(wasmImports);
  // prepare imports
  return {
    'env': wasmImports,
    'wasi_snapshot_preview1': wasmImports,
  }
}

// Create the wasm instance.
// Receives the wasm imports, returns the exports.
async function createWasm() {
  // Load the wasm module and create an instance of using native support in the JS engine.
  // handle a generated wasm instance, receiving its exports and
  // performing other necessary setup
  /** @param {WebAssembly.Module=} module*/
  function receiveInstance(instance, module) {
    wasmExports = instance.exports;

    wasmExports = Asyncify.instrumentWasmExports(wasmExports);

    

    wasmMemory = wasmExports['memory'];
    
    assert(wasmMemory, 'memory not found in wasm exports');
    updateMemoryViews();

    removeRunDependency('wasm-instantiate');
    return wasmExports;
  }
  // wait for the pthread pool (if any)
  addRunDependency('wasm-instantiate');

  // Prefer streaming instantiation if available.
  // Async compilation can be confusing when an error on the page overwrites Module
  // (for example, if the order of elements is wrong, and the one defining Module is
  // later), so we save Module and check it later.
  var trueModule = Module;
  function receiveInstantiationResult(result) {
    // 'result' is a ResultObject object which has both the module and instance.
    // receiveInstance() will swap in the exports (to Module.asm) so they can be called
    assert(Module === trueModule, 'the Module object should not be replaced during async compilation - perhaps the order of HTML elements is wrong?');
    trueModule = null;
    // TODO: Due to Closure regression https://github.com/google/closure-compiler/issues/3193, the above line no longer optimizes out down to the following line.
    // When the regression is fixed, can restore the above PTHREADS-enabled path.
    return receiveInstance(result['instance']);
  }

  var info = getWasmImports();

  // User shell pages can write their own Module.instantiateWasm = function(imports, successCallback) callback
  // to manually instantiate the Wasm module themselves. This allows pages to
  // run the instantiation parallel to any other async startup actions they are
  // performing.
  // Also pthreads and wasm workers initialize the wasm instance through this
  // path.
  if (Module['instantiateWasm']) {
    return new Promise((resolve, reject) => {
      try {
        Module['instantiateWasm'](info, (mod, inst) => {
          resolve(receiveInstance(mod, inst));
        });
      } catch(e) {
        err(`Module.instantiateWasm callback failed with error: ${e}`);
        reject(e);
      }
    });
  }

  wasmBinaryFile ??= findWasmBinary();
    var result = await instantiateAsync(wasmBinary, wasmBinaryFile, info);
    var exports = receiveInstantiationResult(result);
    return exports;
}

// end include: preamble.js

// Begin JS library code


  class ExitStatus {
      name = 'ExitStatus';
      constructor(status) {
        this.message = `Program terminated with exit(${status})`;
        this.status = status;
      }
    }

  var callRuntimeCallbacks = (callbacks) => {
      while (callbacks.length > 0) {
        // Pass the module as the first argument.
        callbacks.shift()(Module);
      }
    };
  var onPostRuns = [];
  var addOnPostRun = (cb) => onPostRuns.push(cb);

  var onPreRuns = [];
  var addOnPreRun = (cb) => onPreRuns.push(cb);


  
    /**
     * @param {number} ptr
     * @param {string} type
     */
  function getValue(ptr, type = 'i8') {
    if (type.endsWith('*')) type = '*';
    switch (type) {
      case 'i1': return HEAP8[ptr];
      case 'i8': return HEAP8[ptr];
      case 'i16': return HEAP16[((ptr)>>1)];
      case 'i32': return HEAP32[((ptr)>>2)];
      case 'i64': return HEAP64[((ptr)>>3)];
      case 'float': return HEAPF32[((ptr)>>2)];
      case 'double': return HEAPF64[((ptr)>>3)];
      case '*': return HEAPU32[((ptr)>>2)];
      default: abort(`invalid type for getValue: ${type}`);
    }
  }

  var noExitRuntime = true;

  var ptrToString = (ptr) => {
      assert(typeof ptr === 'number');
      // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
      ptr >>>= 0;
      return '0x' + ptr.toString(16).padStart(8, '0');
    };

  
    /**
     * @param {number} ptr
     * @param {number} value
     * @param {string} type
     */
  function setValue(ptr, value, type = 'i8') {
    if (type.endsWith('*')) type = '*';
    switch (type) {
      case 'i1': HEAP8[ptr] = value; break;
      case 'i8': HEAP8[ptr] = value; break;
      case 'i16': HEAP16[((ptr)>>1)] = value; break;
      case 'i32': HEAP32[((ptr)>>2)] = value; break;
      case 'i64': HEAP64[((ptr)>>3)] = BigInt(value); break;
      case 'float': HEAPF32[((ptr)>>2)] = value; break;
      case 'double': HEAPF64[((ptr)>>3)] = value; break;
      case '*': HEAPU32[((ptr)>>2)] = value; break;
      default: abort(`invalid type for setValue: ${type}`);
    }
  }

  var stackRestore = (val) => __emscripten_stack_restore(val);

  var stackSave = () => _emscripten_stack_get_current();

  var warnOnce = (text) => {
      warnOnce.shown ||= {};
      if (!warnOnce.shown[text]) {
        warnOnce.shown[text] = 1;
        if (ENVIRONMENT_IS_NODE) text = 'warning: ' + text;
        err(text);
      }
    };

  var __abort_js = () =>
      abort('native code called abort()');

  var abortOnCannotGrowMemory = (requestedSize) => {
      abort(`Cannot enlarge memory arrays to size ${requestedSize} bytes (OOM). Either (1) compile with -sINITIAL_MEMORY=X with X higher than the current value ${HEAP8.length}, (2) compile with -sALLOW_MEMORY_GROWTH which allows increasing the size at runtime, or (3) if you want malloc to return NULL (0) instead of this abort, compile with -sABORTING_MALLOC=0`);
    };
  var _emscripten_resize_heap = (requestedSize) => {
      var oldSize = HEAPU8.length;
      // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
      requestedSize >>>= 0;
      abortOnCannotGrowMemory(requestedSize);
    };

  var UTF8Decoder = typeof TextDecoder != 'undefined' ? new TextDecoder() : undefined;
  
    /**
     * Given a pointer 'idx' to a null-terminated UTF8-encoded string in the given
     * array that contains uint8 values, returns a copy of that string as a
     * Javascript String object.
     * heapOrArray is either a regular array, or a JavaScript typed array view.
     * @param {number=} idx
     * @param {number=} maxBytesToRead
     * @return {string}
     */
  var UTF8ArrayToString = (heapOrArray, idx = 0, maxBytesToRead = NaN) => {
      var endIdx = idx + maxBytesToRead;
      var endPtr = idx;
      // TextDecoder needs to know the byte length in advance, it doesn't stop on
      // null terminator by itself.  Also, use the length info to avoid running tiny
      // strings through TextDecoder, since .subarray() allocates garbage.
      // (As a tiny code save trick, compare endPtr against endIdx using a negation,
      // so that undefined/NaN means Infinity)
      while (heapOrArray[endPtr] && !(endPtr >= endIdx)) ++endPtr;
  
      if (endPtr - idx > 16 && heapOrArray.buffer && UTF8Decoder) {
        return UTF8Decoder.decode(heapOrArray.subarray(idx, endPtr));
      }
      var str = '';
      // If building with TextDecoder, we have already computed the string length
      // above, so test loop end condition against that
      while (idx < endPtr) {
        // For UTF8 byte structure, see:
        // http://en.wikipedia.org/wiki/UTF-8#Description
        // https://www.ietf.org/rfc/rfc2279.txt
        // https://tools.ietf.org/html/rfc3629
        var u0 = heapOrArray[idx++];
        if (!(u0 & 0x80)) { str += String.fromCharCode(u0); continue; }
        var u1 = heapOrArray[idx++] & 63;
        if ((u0 & 0xE0) == 0xC0) { str += String.fromCharCode(((u0 & 31) << 6) | u1); continue; }
        var u2 = heapOrArray[idx++] & 63;
        if ((u0 & 0xF0) == 0xE0) {
          u0 = ((u0 & 15) << 12) | (u1 << 6) | u2;
        } else {
          if ((u0 & 0xF8) != 0xF0) warnOnce('Invalid UTF-8 leading byte ' + ptrToString(u0) + ' encountered when deserializing a UTF-8 string in wasm memory to a JS string!');
          u0 = ((u0 & 7) << 18) | (u1 << 12) | (u2 << 6) | (heapOrArray[idx++] & 63);
        }
  
        if (u0 < 0x10000) {
          str += String.fromCharCode(u0);
        } else {
          var ch = u0 - 0x10000;
          str += String.fromCharCode(0xD800 | (ch >> 10), 0xDC00 | (ch & 0x3FF));
        }
      }
      return str;
    };
  
    /**
     * Given a pointer 'ptr' to a null-terminated UTF8-encoded string in the
     * emscripten HEAP, returns a copy of that string as a Javascript String object.
     *
     * @param {number} ptr
     * @param {number=} maxBytesToRead - An optional length that specifies the
     *   maximum number of bytes to read. You can omit this parameter to scan the
     *   string until the first 0 byte. If maxBytesToRead is passed, and the string
     *   at [ptr, ptr+maxBytesToReadr[ contains a null byte in the middle, then the
     *   string will cut short at that byte index (i.e. maxBytesToRead will not
     *   produce a string of exact length [ptr, ptr+maxBytesToRead[) N.B. mixing
     *   frequent uses of UTF8ToString() with and without maxBytesToRead may throw
     *   JS JIT optimizations off, so it is worth to consider consistently using one
     * @return {string}
     */
  var UTF8ToString = (ptr, maxBytesToRead) => {
      assert(typeof ptr == 'number', `UTF8ToString expects a number (got ${typeof ptr})`);
      return ptr ? UTF8ArrayToString(HEAPU8, ptr, maxBytesToRead) : '';
    };
  var SYSCALLS = {
  varargs:undefined,
  getStr(ptr) {
        var ret = UTF8ToString(ptr);
        return ret;
      },
  };
  var _fd_close = (fd) => {
      abort('fd_close called without SYSCALLS_REQUIRE_FILESYSTEM');
    };

  var INT53_MAX = 9007199254740992;
  
  var INT53_MIN = -9007199254740992;
  var bigintToI53Checked = (num) => (num < INT53_MIN || num > INT53_MAX) ? NaN : Number(num);
  function _fd_seek(fd, offset, whence, newOffset) {
    offset = bigintToI53Checked(offset);
  
  
      return 70;
    ;
  }

  var printCharBuffers = [null,[],[]];
  
  var printChar = (stream, curr) => {
      var buffer = printCharBuffers[stream];
      assert(buffer);
      if (curr === 0 || curr === 10) {
        (stream === 1 ? out : err)(UTF8ArrayToString(buffer));
        buffer.length = 0;
      } else {
        buffer.push(curr);
      }
    };
  
  var flush_NO_FILESYSTEM = () => {
      // flush anything remaining in the buffers during shutdown
      _fflush(0);
      if (printCharBuffers[1].length) printChar(1, 10);
      if (printCharBuffers[2].length) printChar(2, 10);
    };
  
  
  var _fd_write = (fd, iov, iovcnt, pnum) => {
      // hack to support printf in SYSCALLS_REQUIRE_FILESYSTEM=0
      var num = 0;
      for (var i = 0; i < iovcnt; i++) {
        var ptr = HEAPU32[((iov)>>2)];
        var len = HEAPU32[(((iov)+(4))>>2)];
        iov += 8;
        for (var j = 0; j < len; j++) {
          printChar(fd, HEAPU8[ptr+j]);
        }
        num += len;
      }
      HEAPU32[((pnum)>>2)] = num;
      return 0;
    };

  
  var runtimeKeepaliveCounter = 0;
  var keepRuntimeAlive = () => noExitRuntime || runtimeKeepaliveCounter > 0;
  var _proc_exit = (code) => {
      EXITSTATUS = code;
      if (!keepRuntimeAlive()) {
        Module['onExit']?.(code);
        ABORT = true;
      }
      quit_(code, new ExitStatus(code));
    };
  
  
  /** @param {boolean|number=} implicit */
  var exitJS = (status, implicit) => {
      EXITSTATUS = status;
  
      checkUnflushedContent();
  
      // if exit() was called explicitly, warn the user if the runtime isn't actually being shut down
      if (keepRuntimeAlive() && !implicit) {
        var msg = `program exited (with status: ${status}), but keepRuntimeAlive() is set (counter=${runtimeKeepaliveCounter}) due to an async operation, so halting execution but not exiting the runtime or preventing further async execution (you can use emscripten_force_exit, if you want to force a true shutdown)`;
        err(msg);
      }
  
      _proc_exit(status);
    };

  var handleException = (e) => {
      // Certain exception types we do not treat as errors since they are used for
      // internal control flow.
      // 1. ExitStatus, which is thrown by exit()
      // 2. "unwind", which is thrown by emscripten_unwind_to_js_event_loop() and others
      //    that wish to return to JS event loop.
      if (e instanceof ExitStatus || e == 'unwind') {
        return EXITSTATUS;
      }
      checkStackCookie();
      if (e instanceof WebAssembly.RuntimeError) {
        if (_emscripten_stack_get_current() <= 0) {
          err('Stack overflow detected.  You can try increasing -sSTACK_SIZE (currently set to 65536)');
        }
      }
      quit_(1, e);
    };

  var runAndAbortIfError = (func) => {
      try {
        return func();
      } catch (e) {
        abort(e);
      }
    };
  
  
  var _exit = exitJS;
  
  
  var maybeExit = () => {
      if (!keepRuntimeAlive()) {
        try {
          _exit(EXITSTATUS);
        } catch (e) {
          handleException(e);
        }
      }
    };
  var callUserCallback = (func) => {
      if (ABORT) {
        err('user callback triggered after runtime exited or application aborted.  Ignoring.');
        return;
      }
      try {
        func();
        maybeExit();
      } catch (e) {
        handleException(e);
      }
    };
  
  var sigToWasmTypes = (sig) => {
      var typeNames = {
        'i': 'i32',
        'j': 'i64',
        'f': 'f32',
        'd': 'f64',
        'e': 'externref',
        'p': 'i32',
      };
      var type = {
        parameters: [],
        results: sig[0] == 'v' ? [] : [typeNames[sig[0]]]
      };
      for (var i = 1; i < sig.length; ++i) {
        assert(sig[i] in typeNames, 'invalid signature char: ' + sig[i]);
        type.parameters.push(typeNames[sig[i]]);
      }
      return type;
    };
  
  var runtimeKeepalivePush = () => {
      runtimeKeepaliveCounter += 1;
    };
  
  var runtimeKeepalivePop = () => {
      assert(runtimeKeepaliveCounter > 0);
      runtimeKeepaliveCounter -= 1;
    };
  
  
  var Asyncify = {
  instrumentWasmImports(imports) {
        var importPattern = /^(invoke_.*|__asyncjs__.*)$/;
  
        for (let [x, original] of Object.entries(imports)) {
          if (typeof original == 'function') {
            let isAsyncifyImport = original.isAsync || importPattern.test(x);
            imports[x] = (...args) => {
              var originalAsyncifyState = Asyncify.state;
              try {
                return original(...args);
              } finally {
                // Only asyncify-declared imports are allowed to change the
                // state.
                // Changing the state from normal to disabled is allowed (in any
                // function) as that is what shutdown does (and we don't have an
                // explicit list of shutdown imports).
                var changedToDisabled =
                      originalAsyncifyState === Asyncify.State.Normal &&
                      Asyncify.state        === Asyncify.State.Disabled;
                // invoke_* functions are allowed to change the state if we do
                // not ignore indirect calls.
                var ignoredInvoke = x.startsWith('invoke_') &&
                                    true;
                if (Asyncify.state !== originalAsyncifyState &&
                    !isAsyncifyImport &&
                    !changedToDisabled &&
                    !ignoredInvoke) {
                  throw new Error(`import ${x} was not in ASYNCIFY_IMPORTS, but changed the state`);
                }
              }
            };
          }
        }
      },
  instrumentWasmExports(exports) {
        var ret = {};
        for (let [x, original] of Object.entries(exports)) {
          if (typeof original == 'function') {
            ret[x] = (...args) => {
              Asyncify.exportCallStack.push(x);
              try {
                return original(...args);
              } finally {
                if (!ABORT) {
                  var y = Asyncify.exportCallStack.pop();
                  assert(y === x);
                  Asyncify.maybeStopUnwind();
                }
              }
            };
          } else {
            ret[x] = original;
          }
        }
        return ret;
      },
  State:{
  Normal:0,
  Unwinding:1,
  Rewinding:2,
  Disabled:3,
  },
  state:0,
  StackSize:4096,
  currData:null,
  handleSleepReturnValue:0,
  exportCallStack:[],
  callStackNameToId:{
  },
  callStackIdToName:{
  },
  callStackId:0,
  asyncPromiseHandlers:null,
  sleepCallbacks:[],
  getCallStackId(funcName) {
        var id = Asyncify.callStackNameToId[funcName];
        if (id === undefined) {
          id = Asyncify.callStackId++;
          Asyncify.callStackNameToId[funcName] = id;
          Asyncify.callStackIdToName[id] = funcName;
        }
        return id;
      },
  maybeStopUnwind() {
        if (Asyncify.currData &&
            Asyncify.state === Asyncify.State.Unwinding &&
            Asyncify.exportCallStack.length === 0) {
          // We just finished unwinding.
          // Be sure to set the state before calling any other functions to avoid
          // possible infinite recursion here (For example in debug pthread builds
          // the dbg() function itself can call back into WebAssembly to get the
          // current pthread_self() pointer).
          Asyncify.state = Asyncify.State.Normal;
          
          // Keep the runtime alive so that a re-wind can be done later.
          runAndAbortIfError(_asyncify_stop_unwind);
          if (typeof Fibers != 'undefined') {
            Fibers.trampoline();
          }
        }
      },
  whenDone() {
        assert(Asyncify.currData, 'Tried to wait for an async operation when none is in progress.');
        assert(!Asyncify.asyncPromiseHandlers, 'Cannot have multiple async operations in flight at once');
        return new Promise((resolve, reject) => {
          Asyncify.asyncPromiseHandlers = { resolve, reject };
        });
      },
  allocateData() {
        // An asyncify data structure has three fields:
        //  0  current stack pos
        //  4  max stack pos
        //  8  id of function at bottom of the call stack (callStackIdToName[id] == name of js function)
        //
        // The Asyncify ABI only interprets the first two fields, the rest is for the runtime.
        // We also embed a stack in the same memory region here, right next to the structure.
        // This struct is also defined as asyncify_data_t in emscripten/fiber.h
        var ptr = _malloc(12 + Asyncify.StackSize);
        Asyncify.setDataHeader(ptr, ptr + 12, Asyncify.StackSize);
        Asyncify.setDataRewindFunc(ptr);
        return ptr;
      },
  setDataHeader(ptr, stack, stackSize) {
        HEAPU32[((ptr)>>2)] = stack;
        HEAPU32[(((ptr)+(4))>>2)] = stack + stackSize;
      },
  setDataRewindFunc(ptr) {
        var bottomOfCallStack = Asyncify.exportCallStack[0];
        var rewindId = Asyncify.getCallStackId(bottomOfCallStack);
        HEAP32[(((ptr)+(8))>>2)] = rewindId;
      },
  getDataRewindFuncName(ptr) {
        var id = HEAP32[(((ptr)+(8))>>2)];
        var name = Asyncify.callStackIdToName[id];
        return name;
      },
  getDataRewindFunc(name) {
        var func = wasmExports[name];
        return func;
      },
  doRewind(ptr) {
        var name = Asyncify.getDataRewindFuncName(ptr);
        var func = Asyncify.getDataRewindFunc(name);
        // Once we have rewound and the stack we no longer need to artificially
        // keep the runtime alive.
        
        return func();
      },
  handleSleep(startAsync) {
        assert(Asyncify.state !== Asyncify.State.Disabled, 'Asyncify cannot be done during or after the runtime exits');
        if (ABORT) return;
        if (Asyncify.state === Asyncify.State.Normal) {
          // Prepare to sleep. Call startAsync, and see what happens:
          // if the code decided to call our callback synchronously,
          // then no async operation was in fact begun, and we don't
          // need to do anything.
          var reachedCallback = false;
          var reachedAfterCallback = false;
          startAsync((handleSleepReturnValue = 0) => {
            assert(!handleSleepReturnValue || typeof handleSleepReturnValue == 'number' || typeof handleSleepReturnValue == 'boolean'); // old emterpretify API supported other stuff
            if (ABORT) return;
            Asyncify.handleSleepReturnValue = handleSleepReturnValue;
            reachedCallback = true;
            if (!reachedAfterCallback) {
              // We are happening synchronously, so no need for async.
              return;
            }
            // This async operation did not happen synchronously, so we did
            // unwind. In that case there can be no compiled code on the stack,
            // as it might break later operations (we can rewind ok now, but if
            // we unwind again, we would unwind through the extra compiled code
            // too).
            assert(!Asyncify.exportCallStack.length, 'Waking up (starting to rewind) must be done from JS, without compiled code on the stack.');
            Asyncify.state = Asyncify.State.Rewinding;
            runAndAbortIfError(() => _asyncify_start_rewind(Asyncify.currData));
            if (typeof MainLoop != 'undefined' && MainLoop.func) {
              MainLoop.resume();
            }
            var asyncWasmReturnValue, isError = false;
            try {
              asyncWasmReturnValue = Asyncify.doRewind(Asyncify.currData);
            } catch (err) {
              asyncWasmReturnValue = err;
              isError = true;
            }
            // Track whether the return value was handled by any promise handlers.
            var handled = false;
            if (!Asyncify.currData) {
              // All asynchronous execution has finished.
              // `asyncWasmReturnValue` now contains the final
              // return value of the exported async WASM function.
              //
              // Note: `asyncWasmReturnValue` is distinct from
              // `Asyncify.handleSleepReturnValue`.
              // `Asyncify.handleSleepReturnValue` contains the return
              // value of the last C function to have executed
              // `Asyncify.handleSleep()`, where as `asyncWasmReturnValue`
              // contains the return value of the exported WASM function
              // that may have called C functions that
              // call `Asyncify.handleSleep()`.
              var asyncPromiseHandlers = Asyncify.asyncPromiseHandlers;
              if (asyncPromiseHandlers) {
                Asyncify.asyncPromiseHandlers = null;
                (isError ? asyncPromiseHandlers.reject : asyncPromiseHandlers.resolve)(asyncWasmReturnValue);
                handled = true;
              }
            }
            if (isError && !handled) {
              // If there was an error and it was not handled by now, we have no choice but to
              // rethrow that error into the global scope where it can be caught only by
              // `onerror` or `onunhandledpromiserejection`.
              throw asyncWasmReturnValue;
            }
          });
          reachedAfterCallback = true;
          if (!reachedCallback) {
            // A true async operation was begun; start a sleep.
            Asyncify.state = Asyncify.State.Unwinding;
            // TODO: reuse, don't alloc/free every sleep
            Asyncify.currData = Asyncify.allocateData();
            if (typeof MainLoop != 'undefined' && MainLoop.func) {
              MainLoop.pause();
            }
            runAndAbortIfError(() => _asyncify_start_unwind(Asyncify.currData));
          }
        } else if (Asyncify.state === Asyncify.State.Rewinding) {
          // Stop a resume.
          Asyncify.state = Asyncify.State.Normal;
          runAndAbortIfError(_asyncify_stop_rewind);
          _free(Asyncify.currData);
          Asyncify.currData = null;
          // Call all sleep callbacks now that the sleep-resume is all done.
          Asyncify.sleepCallbacks.forEach(callUserCallback);
        } else {
          abort(`invalid state: ${Asyncify.state}`);
        }
        return Asyncify.handleSleepReturnValue;
      },
  handleAsync(startAsync) {
        return Asyncify.handleSleep((wakeUp) => {
          // TODO: add error handling as a second param when handleSleep implements it.
          startAsync().then(wakeUp);
        });
      },
  };
// End JS library code

// include: postlibrary.js
// This file is included after the automatically-generated JS library code
// but before the wasm module is created.

{

  // Begin ATMODULES hooks
  if (Module['noExitRuntime']) noExitRuntime = Module['noExitRuntime'];
if (Module['print']) out = Module['print'];
if (Module['printErr']) err = Module['printErr'];
if (Module['wasmBinary']) wasmBinary = Module['wasmBinary'];

Module['FS_createDataFile'] = FS.createDataFile;
Module['FS_createPreloadedFile'] = FS.createPreloadedFile;

  // End ATMODULES hooks

  checkIncomingModuleAPI();

  if (Module['arguments']) arguments_ = Module['arguments'];
  if (Module['thisProgram']) thisProgram = Module['thisProgram'];

  // Assertions on removed incoming Module JS APIs.
  assert(typeof Module['memoryInitializerPrefixURL'] == 'undefined', 'Module.memoryInitializerPrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['pthreadMainPrefixURL'] == 'undefined', 'Module.pthreadMainPrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['cdInitializerPrefixURL'] == 'undefined', 'Module.cdInitializerPrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['filePackagePrefixURL'] == 'undefined', 'Module.filePackagePrefixURL option was removed, use Module.locateFile instead');
  assert(typeof Module['read'] == 'undefined', 'Module.read option was removed');
  assert(typeof Module['readAsync'] == 'undefined', 'Module.readAsync option was removed (modify readAsync in JS)');
  assert(typeof Module['readBinary'] == 'undefined', 'Module.readBinary option was removed (modify readBinary in JS)');
  assert(typeof Module['setWindowTitle'] == 'undefined', 'Module.setWindowTitle option was removed (modify emscripten_set_window_title in JS)');
  assert(typeof Module['TOTAL_MEMORY'] == 'undefined', 'Module.TOTAL_MEMORY has been renamed Module.INITIAL_MEMORY');
  assert(typeof Module['ENVIRONMENT'] == 'undefined', 'Module.ENVIRONMENT has been deprecated. To force the environment, use the ENVIRONMENT compile-time option (for example, -sENVIRONMENT=web or -sENVIRONMENT=node)');
  assert(typeof Module['STACK_SIZE'] == 'undefined', 'STACK_SIZE can no longer be set at runtime.  Use -sSTACK_SIZE at link time')
  // If memory is defined in wasm, the user can't provide it, or set INITIAL_MEMORY
  assert(typeof Module['wasmMemory'] == 'undefined', 'Use of `wasmMemory` detected.  Use -sIMPORTED_MEMORY to define wasmMemory externally');
  assert(typeof Module['INITIAL_MEMORY'] == 'undefined', 'Detected runtime INITIAL_MEMORY setting.  Use -sIMPORTED_MEMORY to define wasmMemory dynamically');

}

// Begin runtime exports
  var missingLibrarySymbols = [
  'writeI53ToI64',
  'writeI53ToI64Clamped',
  'writeI53ToI64Signaling',
  'writeI53ToU64Clamped',
  'writeI53ToU64Signaling',
  'readI53FromI64',
  'readI53FromU64',
  'convertI32PairToI53',
  'convertI32PairToI53Checked',
  'convertU32PairToI53',
  'stackAlloc',
  'getTempRet0',
  'setTempRet0',
  'zeroMemory',
  'getHeapMax',
  'growMemory',
  'strError',
  'inetPton4',
  'inetNtop4',
  'inetPton6',
  'inetNtop6',
  'readSockaddr',
  'writeSockaddr',
  'emscriptenLog',
  'readEmAsmArgs',
  'jstoi_q',
  'getExecutableName',
  'listenOnce',
  'autoResumeAudioContext',
  'dynCallLegacy',
  'getDynCaller',
  'dynCall',
  'asmjsMangle',
  'asyncLoad',
  'alignMemory',
  'mmapAlloc',
  'HandleAllocator',
  'getNativeTypeSize',
  'addOnInit',
  'addOnPostCtor',
  'addOnPreMain',
  'addOnExit',
  'STACK_SIZE',
  'STACK_ALIGN',
  'POINTER_SIZE',
  'ASSERTIONS',
  'getCFunc',
  'ccall',
  'cwrap',
  'uleb128Encode',
  'generateFuncType',
  'convertJsFunctionToWasm',
  'getEmptyTableSlot',
  'updateTableMap',
  'getFunctionAddress',
  'addFunction',
  'removeFunction',
  'reallyNegative',
  'unSign',
  'strLen',
  'reSign',
  'formatString',
  'stringToUTF8Array',
  'stringToUTF8',
  'lengthBytesUTF8',
  'intArrayFromString',
  'intArrayToString',
  'AsciiToString',
  'stringToAscii',
  'UTF16ToString',
  'stringToUTF16',
  'lengthBytesUTF16',
  'UTF32ToString',
  'stringToUTF32',
  'lengthBytesUTF32',
  'stringToNewUTF8',
  'stringToUTF8OnStack',
  'writeArrayToMemory',
  'registerKeyEventCallback',
  'maybeCStringToJsString',
  'findEventTarget',
  'getBoundingClientRect',
  'fillMouseEventData',
  'registerMouseEventCallback',
  'registerWheelEventCallback',
  'registerUiEventCallback',
  'registerFocusEventCallback',
  'fillDeviceOrientationEventData',
  'registerDeviceOrientationEventCallback',
  'fillDeviceMotionEventData',
  'registerDeviceMotionEventCallback',
  'screenOrientation',
  'fillOrientationChangeEventData',
  'registerOrientationChangeEventCallback',
  'fillFullscreenChangeEventData',
  'registerFullscreenChangeEventCallback',
  'JSEvents_requestFullscreen',
  'JSEvents_resizeCanvasForFullscreen',
  'registerRestoreOldStyle',
  'hideEverythingExceptGivenElement',
  'restoreHiddenElements',
  'setLetterbox',
  'softFullscreenResizeWebGLRenderTarget',
  'doRequestFullscreen',
  'fillPointerlockChangeEventData',
  'registerPointerlockChangeEventCallback',
  'registerPointerlockErrorEventCallback',
  'requestPointerLock',
  'fillVisibilityChangeEventData',
  'registerVisibilityChangeEventCallback',
  'registerTouchEventCallback',
  'fillGamepadEventData',
  'registerGamepadEventCallback',
  'registerBeforeUnloadEventCallback',
  'fillBatteryEventData',
  'battery',
  'registerBatteryEventCallback',
  'setCanvasElementSize',
  'getCanvasElementSize',
  'jsStackTrace',
  'getCallstack',
  'convertPCtoSourceLocation',
  'getEnvStrings',
  'checkWasiClock',
  'wasiRightsToMuslOFlags',
  'wasiOFlagsToMuslOFlags',
  'initRandomFill',
  'randomFill',
  'safeSetTimeout',
  'setImmediateWrapped',
  'safeRequestAnimationFrame',
  'clearImmediateWrapped',
  'registerPostMainLoop',
  'registerPreMainLoop',
  'getPromise',
  'makePromise',
  'idsToPromises',
  'makePromiseCallback',
  'ExceptionInfo',
  'findMatchingCatch',
  'Browser_asyncPrepareDataCounter',
  'isLeapYear',
  'ydayFromDate',
  'arraySum',
  'addDays',
  'getSocketFromFD',
  'getSocketAddress',
  'FS_createPreloadedFile',
  'FS_modeStringToFlags',
  'FS_getMode',
  'FS_stdin_getChar',
  'FS_unlink',
  'FS_createDataFile',
  'FS_mkdirTree',
  '_setNetworkCallback',
  'heapObjectForWebGLType',
  'toTypedArrayIndex',
  'webgl_enable_ANGLE_instanced_arrays',
  'webgl_enable_OES_vertex_array_object',
  'webgl_enable_WEBGL_draw_buffers',
  'webgl_enable_WEBGL_multi_draw',
  'webgl_enable_EXT_polygon_offset_clamp',
  'webgl_enable_EXT_clip_control',
  'webgl_enable_WEBGL_polygon_mode',
  'emscriptenWebGLGet',
  'computeUnpackAlignedImageSize',
  'colorChannelsInGlTextureFormat',
  'emscriptenWebGLGetTexPixelData',
  'emscriptenWebGLGetUniform',
  'webglGetUniformLocation',
  'webglPrepareUniformLocationsBeforeFirstUse',
  'webglGetLeftBracePos',
  'emscriptenWebGLGetVertexAttrib',
  '__glGetActiveAttribOrUniform',
  'writeGLArray',
  'registerWebGlEventCallback',
  'ALLOC_NORMAL',
  'ALLOC_STACK',
  'allocate',
  'writeStringToMemory',
  'writeAsciiToMemory',
  'demangle',
  'stackTrace',
];
missingLibrarySymbols.forEach(missingLibrarySymbol)

  var unexportedSymbols = [
  'run',
  'addRunDependency',
  'removeRunDependency',
  'out',
  'err',
  'callMain',
  'abort',
  'wasmMemory',
  'wasmExports',
  'HEAPF32',
  'HEAPF64',
  'HEAP8',
  'HEAPU8',
  'HEAP16',
  'HEAPU16',
  'HEAP32',
  'HEAPU32',
  'HEAP64',
  'HEAPU64',
  'writeStackCookie',
  'checkStackCookie',
  'INT53_MAX',
  'INT53_MIN',
  'bigintToI53Checked',
  'stackSave',
  'stackRestore',
  'ptrToString',
  'exitJS',
  'abortOnCannotGrowMemory',
  'ENV',
  'ERRNO_CODES',
  'DNS',
  'Protocols',
  'Sockets',
  'timers',
  'warnOnce',
  'readEmAsmArgsArray',
  'jstoi_s',
  'handleException',
  'keepRuntimeAlive',
  'runtimeKeepalivePush',
  'runtimeKeepalivePop',
  'callUserCallback',
  'maybeExit',
  'wasmTable',
  'noExitRuntime',
  'addOnPreRun',
  'addOnPostRun',
  'sigToWasmTypes',
  'freeTableIndexes',
  'functionsInTableMap',
  'setValue',
  'getValue',
  'PATH',
  'PATH_FS',
  'UTF8Decoder',
  'UTF8ArrayToString',
  'UTF8ToString',
  'UTF16Decoder',
  'JSEvents',
  'specialHTMLTargets',
  'findCanvasEventTarget',
  'currentFullscreenStrategy',
  'restoreOldWindowedStyle',
  'UNWIND_CACHE',
  'ExitStatus',
  'flush_NO_FILESYSTEM',
  'emSetImmediate',
  'emClearImmediate_deps',
  'emClearImmediate',
  'promiseMap',
  'uncaughtExceptionCount',
  'exceptionLast',
  'exceptionCaught',
  'Browser',
  'getPreloadedImageData__data',
  'wget',
  'MONTH_DAYS_REGULAR',
  'MONTH_DAYS_LEAP',
  'MONTH_DAYS_REGULAR_CUMULATIVE',
  'MONTH_DAYS_LEAP_CUMULATIVE',
  'SYSCALLS',
  'preloadPlugins',
  'FS_stdin_getChar_buffer',
  'FS_createPath',
  'FS_createDevice',
  'FS_readFile',
  'FS',
  'FS_createLazyFile',
  'MEMFS',
  'TTY',
  'PIPEFS',
  'SOCKFS',
  'tempFixedLengthArray',
  'miniTempWebGLFloatBuffers',
  'miniTempWebGLIntBuffers',
  'GL',
  'AL',
  'GLUT',
  'EGL',
  'GLEW',
  'IDBStore',
  'runAndAbortIfError',
  'Asyncify',
  'Fibers',
  'SDL',
  'SDL_gfx',
  'allocateUTF8',
  'allocateUTF8OnStack',
  'print',
  'printErr',
];
unexportedSymbols.forEach(unexportedRuntimeSymbol);

  // End runtime exports
  // Begin JS library exports
  // End JS library exports

// end include: postlibrary.js

function checkIncomingModuleAPI() {
  ignoredModuleProp('fetchSettings');
}
function c2wasm_append_array_long(stack_index,value) { let array = window.c2wasm_stack[stack_index]; array.push(value); }
function c2wasm_append_array_double(stack_index,value) { let array = window.c2wasm_stack[stack_index]; array.push(value); }
function c2wasm_append_array_string(stack_index,value) { let array = window.c2wasm_stack[stack_index]; array.push(window.c2wasm_get_string(value)); }
function c2wasm_append_array_any(stack_index,stack_index_value) { let array = window.c2wasm_stack[stack_index]; array.push(window.c2wasm_stack[stack_index_value]); }
function c2wasm_append_array_bool(stack_index,value) { let array = window.c2wasm_stack[stack_index]; array.push(value ? true : false); }
function c2wasm_get_array_size(stack_index) { let array = window.c2wasm_stack[stack_index]; return array.length; }
function c2wasm_get_array_long_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; return array[index]; }
function c2wasm_get_array_double_by_index(stack_index,index,dest,size) { let array = window.c2wasm_stack[stack_index]; return array[index]; }
function c2wasm_get_array_string_size_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; let value = array[index]; return value.length; }
function c2wasm_array_memcpy_string(stack_index,index,start_string,dest,size) { let array = window.c2wasm_stack[stack_index]; let value = array[index]; for(let i = 0; i < size; i++){ let current_char = value.charCodeAt(i+start_string); if(isNaN(current_char)){ break; } wasmExports.c2wasm_set_char(dest,i,current_char); } return dest; }
function c2wasm_get_array_any_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; let value = array[index]; if(value == false){ return window.c2wasm_false; } if(value == true){ return window.c2wasm_true; } if(value == null){ return window.c2wasm_null; } if(value == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = value; return created_index; }
function c2wasm_get_array_type_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; let value = array[index]; return window.c2wasm_get_type(value); }
function c2wasm_get_array_bool_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; let value = array[index]; if(value == false){ return 0; } return 1; }
function c2wasm_insert_array_long_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; array.splice(index, 0, value); }
function c2wasm_insert_array_double_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; array.splice(index, 0, value); }
function c2wasm_insert_array_string_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; array.splice(index, 0, window.c2wasm_get_string(value)); }
function c2wasm_insert_array_any_by_index(stack_index,index,stack_index_value) { let array = window.c2wasm_stack[stack_index]; array.splice(index, 0, window.c2wasm_stack[stack_index_value]); }
function c2wasm_insert_array_bool_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; array.splice(index, 0, value ? true : false); }
function c2wasm_pop_array_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; array.splice(index, 1); }
function c2wasm_set_array_long_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; array[index] = value; }
function c2wasm_set_array_double_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; array[index] = value; }
function c2wasm_set_array_string_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; array[index] = window.c2wasm_get_string(value); }
function c2wasm_set_array_any_by_index(stack_index,index,stack_index_value) { let array = window.c2wasm_stack[stack_index]; array[index] = window.c2wasm_stack[stack_index_value]; }
function c2wasm_set_array_bool_by_index(stack_index,index,value) { let array = window.c2wasm_stack[stack_index]; if (value == 0){ array[index] = false; } if (value > 0){ array[index] = true; } }
function c2wasm_set_array_null_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; array[index] = null; }
function c2wasm_set_array_undefined_by_index(stack_index,index) { let array = window.c2wasm_stack[stack_index]; array[index] = undefined; }
function c2wasm_create_bool(value) { let index = window.c2wasm_get_stack_point(); if (value == 0){ window.c2wasm_stack[index] = false; } if (value > 0){ window.c2wasm_stack[index] = true; } return index; }
function c2wasm_create_long(value) { let index = window.c2wasm_get_stack_point(); window.c2wasm_stack[index] = value; return index; }
function c2wasm_create_double(value) { let index = window.c2wasm_get_stack_point(); window.c2wasm_stack[index] = value; return index; }
function c2wasm_create_object() { let index = window.c2wasm_get_stack_point(); window.c2wasm_stack[index] = {}; return index; }
function c2wasm_create_array() { let index = window.c2wasm_get_stack_point(); window.c2wasm_stack[index] = []; return index; }
function c2wasm_create_string(value) { let index = window.c2wasm_get_stack_point(); window.c2wasm_stack[index] = window.c2wasm_get_string(value); return index; }
function c2wasm_create_function_with_internal_argsraw(internal_args,callback) { let internal_value = window.c2wasm_stack[internal_args]; let index = window.c2wasm_get_stack_point(); window.c2wasm_stack[index] = window.c2wasm_create_js_c_interop_callback_with_internal_arg(callback,internal_value); return index; }
function c2wasm_create_function_raw(callback) { let index = window.c2wasm_get_stack_point(); window.c2wasm_stack[index] = window.c2wasm_create_js_c_interop_callback(callback); return index; }
function c2wasm_get_string_len(string_index) { return window.c2wasm_stack[string_index].length; }
function c2wasm_memcpy_string(stack_index,start_string,dest,size) { let value = window.c2wasm_stack[stack_index]; for(let i = 0; i < size; i++){ let current_char = value.charCodeAt(i+start_string); if(isNaN(current_char)){ break; } wasmExports.c2wasm_set_char(dest,i,current_char); } return dest; }
function c2wasm_free(stack_index) { if(window.c2wasm_stack.length <= stack_index){ return; } if(window.c2wasm_local_stack){ let index_of_stack = window.c2wasm_local_stack.indexOf(stack_index); window.c2wasm_local_stack.splice(index_of_stack, 1); } delete window.c2wasm_stack[stack_index]; }
function c2wasm_start() { if (window.c2wasm_started){ return; } window.c2wasm_started = true; window.c2wasm_false = 0; window.c2wasm_true = 1; window.c2wasm_null = 2; window.c2wasm_undefined = 3; window.c2wasm_stack = []; window.c2wasm_stack[0] = false; window.c2wasm_stack[1] = true; window.c2wasm_stack[2] = null; window.c2wasm_stack[3] = undefined; window.c2wasm_stack[4] = arguments; window.c2wasm_stack[5] = window; window.c2wasm_stack[6] = document; window.c2wasm_stack[7] = document.body; window.c2wasm_stack[8] = Error; window.c2wasm_get_string = function(c_str ){ let str_array = []; let size = wasmExports.c2wasm_get_str_size(c_str); for(let i=0; i < size; i++){ let current_char = wasmExports.c2wasm_get_char(c_str,i); str_array[i] = current_char; } let decoder = new TextDecoder('utf-8'); let uint8Array = new Uint8Array(str_array); return decoder.decode(uint8Array); }; window.c2wasm_get_stack_point = function(){ for(let i= 8; i < window.c2wasm_stack.length; i++){ if (window.c2wasm_stack[i] == undefined){ if(window.c2wasm_local_stack){ window.c2wasm_local_stack.push(i); } window.c2wasm_stack[i] = 0; return i; } } window.c2wasm_stack.push(0); let created_index = window.c2wasm_stack.length - 1; if(window.c2wasm_local_stack){ window.c2wasm_local_stack.push(created_index); } return created_index; }; window.c2wasm_create_js_c_interop_callback_with_internal_arg = function(callback,internal_value){ return function(){ let ARGUMENTS_STACK_INDEX = 4; let old_arguments = window.c2wasm_stack[ARGUMENTS_STACK_INDEX]; window.c2wasm_stack[ARGUMENTS_STACK_INDEX] = arguments; let old_local_stack = window.c2wasm_local_stack; let current_local_stack = []; window.c2wasm_local_stack = current_local_stack; let new_interal_args_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[new_interal_args_index] = internal_value; let return_index = wasmExports.c2wasm_call_c_function_with_internal_args(new_interal_args_index,callback); let return_value = window.c2wasm_stack[return_index]; for(let i = 0; i < current_local_stack.length; i++){ let item_to_remove_from_stack = current_local_stack[i]; delete window.c2wasm_stack[item_to_remove_from_stack]; } window.c2wasm_stack[ARGUMENTS_STACK_INDEX] = old_arguments; window.c2wasm_local_stack = old_local_stack; if (return_value instanceof Error){ throw return_value; } return return_value; }; }; window.c2wasm_create_js_c_interop_callback = function(callback){ return function(){ let ARGUMENTS_STACK_INDEX = 4; let old_arguments = window.c2wasm_stack[ARGUMENTS_STACK_INDEX]; window.c2wasm_stack[ARGUMENTS_STACK_INDEX] = arguments; let old_local_stack = window.c2wasm_local_stack; let current_local_stack = []; window.c2wasm_local_stack = current_local_stack; let return_index = wasmExports.c2wasm_call_c_function(callback); let return_value = window.c2wasm_stack[return_index]; for(let i = 0; i < current_local_stack.length; i++){ let item_to_remove_from_stack = current_local_stack[i]; delete window.c2wasm_stack[item_to_remove_from_stack]; } window.c2wasm_stack[ARGUMENTS_STACK_INDEX] = old_arguments; window.c2wasm_local_stack = old_local_stack; if( return_value instanceof Error){ throw return_value; } return return_value; }; }; window.c2wasm_get_type = function(value){ if(value == undefined){ return 0; }; if(value == null){ return 1; }; if(value == true || value == false){ return 2; }; if(typeof value == "number"){ return 3; }; if(typeof value == "string"){ return 4; }; if(Array.isArray(value)){ return 6; }; if(typeof value == "object"){ return 5; }; if(typeof value == "function"){ return 7; }; if(typeof value == "symbol"){ return 8; } if(typeof value == "bigint"){ return 9; }; return -1; }; }
function c2wasm_call_object_prop(stack_index,prop_name,args) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let arguments_callback = null; if(args < 0){ arguments_callback = []; } else{ arguments_callback = window.c2wasm_stack[args]; if(!Array.isArray(arguments_callback)){ arguments_callback = [arguments_callback]; } } let result = null; try{ result = object[prop_name_formatted](...arguments_callback); }catch(error){ result = error; } if(result == false){ return window.c2wasm_false; } if(result == true){ return window.c2wasm_true; } if(result == null){ return window.c2wasm_null; } if(result == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = result; return created_index; }
function c2wasm_call_object_constructor(stack_index,prop_name,args) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let arguments_callback = null; if(args < 0){ arguments_callback = []; } else{ arguments_callback = window.c2wasm_stack[args]; if(!Array.isArray(arguments_callback)){ arguments_callback = [arguments_callback]; } } let result = null; try{ result = new object[prop_name_formatted](...arguments_callback); }catch(error){ result = error; } if(result == false){ return window.c2wasm_false; } if(result == true){ return window.c2wasm_true; } if(result == null){ return window.c2wasm_null; } if(result == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = result; return created_index; }
function __asyncjs__await_c2wasm_call_object_prop(stack_index,prop_name,args) { return Asyncify.handleAsync(async () => { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let arguments_callback = null; if(args < 0){ arguments_callback = []; } else{ arguments_callback = window.c2wasm_stack[args]; if(!Array.isArray(arguments_callback)){ arguments_callback = [arguments_callback]; } } let result = null; try{ result = await object[prop_name_formatted](...arguments_callback); }catch(error){ result = error; } if(result == false){ return window.c2wasm_false; } if(result == true){ return window.c2wasm_true; } if(result == null){ return window.c2wasm_null; } if(result == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = result; return created_index; }); }
function c2wasm_get_object_prop_long(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); return object[prop_name_formatted]; }
function c2wasm_get_object_string_len_prop(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let value = object[prop_name_formatted]; return value.length; }
function c2wams_object_memcpy_string(stack_index,prop_name,start_string,dest,size) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let value = object[prop_name_formatted]; for(let i = 0; i < size; i++){ let current_char = value.charCodeAt(i+start_string); if(isNaN(current_char)){ break; } wasmExports.c2wasm_set_char(dest,i,current_char); } return dest; }
function c2wasm_get_object_prop_double(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); return object[prop_name_formatted]; }
function c2wasm_get_object_prop_type(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let value = object[prop_name_formatted]; return window.c2wasm_get_type(value); }
function c2wasm_get_object_prop_bool(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let value = object[prop_name_formatted]; if(value == false){ return 0; } return 1; }
function c2wasm_get_object_prop_any(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let value = object[prop_name_formatted]; if(value == false){ return window.c2wasm_false; } if(value == true){ return window.c2wasm_true; } if(value == null){ return window.c2wasm_null; } if(value == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = value; return created_index; }
function c2wasm_delete_object_prop(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); delete object[prop_name_formatted]; }
function c2wasm_set_object_prop_long(stack_index,prop_name,value) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); object[prop_name_formatted] = value; }
function c2wasm_set_object_prop_double(stack_index,prop_name,value) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); object[prop_name_formatted] = value; }
function c2wasm_set_object_prop_string(stack_index,prop_name,value) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); let value_formatted = window.c2wasm_get_string(value); object[prop_name_formatted] = value_formatted; }
function c2wasm_set_object_prop_bool(stack_index,prop_name,value) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); if (value == 0){ object[prop_name_formatted] = false; } if (value > 0){ object[prop_name_formatted] = true; } }
function c2wasm_set_object_prop_null(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); object[prop_name_formatted] = null; }
function c2wasm_set_object_prop_undefined(stack_index,prop_name) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); object[prop_name_formatted] = undefined; }
function c2wasm_set_object_prop_any(stack_index,prop_name,stack_index_value) { let object = window.c2wasm_stack[stack_index]; let prop_name_formatted = window.c2wasm_get_string(prop_name); object[prop_name_formatted] = window.c2wasm_stack[stack_index_value]; }
function private_c2wasm_set_object_prop_function_with_internal_args_raw(stack_index,prop_name,internal_args,callback) { let prop_name_formatted = window.c2wasm_get_string(prop_name); let object = window.c2wasm_stack[stack_index]; let internal_value = window.c2wasm_stack[internal_args]; object[prop_name_formatted] = window.c2wasm_create_js_c_interop_callback_with_internal_arg(callback,internal_value); }
function private_c2wasm_set_object_prop_function_raw(stack_index,prop_name,callback) { let prop_name_formatted = window.c2wasm_get_string(prop_name); let object = window.c2wasm_stack[stack_index]; object[prop_name_formatted] = window.c2wasm_create_js_c_interop_callback(callback); }
function c2wasm_call_var(stack_index,args) { let func = window.c2wasm_stack[stack_index]; let arguments_callback = null; if(args < 0){ arguments_callback = []; } else{ arguments_callback = window.c2wasm_stack[args]; if(!Array.isArray(arguments_callback)){ arguments_callback = [arguments_callback]; } } let result = null; try{ result = func(...arguments_callback); }catch(error){ result = error; } if(result == false){ return window.c2wasm_false; } if(result == true){ return window.c2wasm_true; } if(result == null){ return window.c2wasm_null; } if(result == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = result; return created_index; }
function __asyncjs__await_c2wasm_call_var(stack_index,args) { return Asyncify.handleAsync(async () => { let func = window.c2wasm_stack[stack_index]; let arguments_callback = null; if(args < 0){ arguments_callback = []; } else{ arguments_callback = window.c2wasm_stack[args]; if(!Array.isArray(arguments_callback)){ arguments_callback = [arguments_callback]; } } let result = null; try{ result = await func(...arguments_callback); }catch(error){ result = error; } if(result == false){ return window.c2wasm_false; } if(result == true){ return window.c2wasm_true; } if(result == null){ return window.c2wasm_null; } if(result == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = result; return created_index; }); }
function c2wasm_call_var_constructor(stack_index,args) { let constructor = window.c2wasm_stack[stack_index]; let arguments_callback = null; if(args < 0){ arguments_callback = []; } else{ arguments_callback = window.c2wasm_stack[args]; if(!Array.isArray(arguments_callback)){ arguments_callback = [arguments_callback]; } } let result = null; try{ result = new constructor(...arguments_callback); }catch(error){ result = error; } if(result == false){ return window.c2wasm_false; } if(result == true){ return window.c2wasm_true; } if(result == null){ return window.c2wasm_null; } if(result == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = result; return created_index; }
function __asyncjs__await_c2wasm_call_var_constructor(stack_index,args) { return Asyncify.handleAsync(async () => { let constructor = window.c2wasm_stack[stack_index]; let arguments_callback = null; if(args < 0){ arguments_callback = []; } else{ arguments_callback = window.c2wasm_stack[args]; if(!Array.isArray(arguments_callback)){ arguments_callback = [arguments_callback]; } } let result = null; try{ result = await new constructor(...arguments_callback); }catch(error){ result = error; } if(result == false){ return window.c2wasm_false; } if(result == true){ return window.c2wasm_true; } if(result == null){ return window.c2wasm_null; } if(result == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = result; return created_index; }); }
function c2wasm_get_var_long(stack_index) { let value = window.c2wasm_stack[stack_index]; return value; }
function c2wasm_get_var_string_len(stack_index) { let value = window.c2wasm_stack[stack_index]; return value.length; }
function c2wasm_var_memcpy_string(stack_index,start_string,dest,size) { let value = window.c2wasm_stack[stack_index]; for(let i = 0; i < size; i++){ let current_char = value.charCodeAt(i+start_string); if(isNaN(current_char)){ break; } wasmExports.c2wasm_set_char(dest,i,current_char); } return dest; }
function c2wasm_get_var_double(stack_index) { let value = window.c2wasm_stack[stack_index]; return value; }
function c2wasm_get_var_type(stack_index) { let value = window.c2wasm_stack[stack_index]; return window.c2wasm_get_type(value); }
function c2wasm_instance_of(stack_index,target) { let value = window.c2wasm_stack[stack_index]; let target_value = window.c2wasm_stack[target]; return value instanceof target_value ? 1 : 0; }
function c2wasm_get_var_bool(stack_index) { let value = window.c2wasm_stack[stack_index]; if(value == false){ return 0; } return 1; }
function c2wasm_get_var_any(stack_index) { let value = window.c2wasm_stack[stack_index]; if(value == false){ return window.c2wasm_false; } if(value == true){ return window.c2wasm_true; } if(value == null){ return window.c2wasm_null; } if(value == undefined){ return window.c2wasm_undefined; } let created_index = window.c2wasm_get_stack_point(); window.c2wasm_stack[created_index] = value; return created_index; }
function c2wasm_show_var_on_console(stack_index) { let element = window.c2wasm_stack[stack_index]; console.log(element); }
var wasmImports = {
  /** @export */
  __asyncjs__await_c2wasm_call_object_prop,
  /** @export */
  _abort_js: __abort_js,
  /** @export */
  c2wasm_append_array_any,
  /** @export */
  c2wasm_append_array_string,
  /** @export */
  c2wasm_array_memcpy_string,
  /** @export */
  c2wasm_call_object_constructor,
  /** @export */
  c2wasm_call_object_prop,
  /** @export */
  c2wasm_create_array,
  /** @export */
  c2wasm_create_object,
  /** @export */
  c2wasm_create_string,
  /** @export */
  c2wasm_get_array_any_by_index,
  /** @export */
  c2wasm_get_array_long_by_index,
  /** @export */
  c2wasm_get_array_size,
  /** @export */
  c2wasm_get_array_string_size_by_index,
  /** @export */
  c2wasm_get_object_prop_any,
  /** @export */
  c2wasm_get_object_prop_long,
  /** @export */
  c2wasm_get_var_string_len,
  /** @export */
  c2wasm_instance_of,
  /** @export */
  c2wasm_memcpy_string,
  /** @export */
  c2wasm_set_object_prop_any,
  /** @export */
  c2wasm_set_object_prop_string,
  /** @export */
  c2wasm_start,
  /** @export */
  emscripten_resize_heap: _emscripten_resize_heap,
  /** @export */
  fd_close: _fd_close,
  /** @export */
  fd_seek: _fd_seek,
  /** @export */
  fd_write: _fd_write,
  /** @export */
  private_c2wasm_set_object_prop_function_raw,
  /** @export */
  private_c2wasm_set_object_prop_function_with_internal_args_raw
};
var wasmExports;
createWasm();
var ___wasm_call_ctors = createExportWrapper('__wasm_call_ctors', 0);
var _c2wasm_get_char = Module['_c2wasm_get_char'] = createExportWrapper('c2wasm_get_char', 2);
var _c2wasm_set_char = Module['_c2wasm_set_char'] = createExportWrapper('c2wasm_set_char', 3);
var _c2wasm_get_str_size = Module['_c2wasm_get_str_size'] = createExportWrapper('c2wasm_get_str_size', 1);
var _c2wasm_call_c_function_with_internal_args = Module['_c2wasm_call_c_function_with_internal_args'] = createExportWrapper('c2wasm_call_c_function_with_internal_args', 2);
var _c2wasm_call_c_function = Module['_c2wasm_call_c_function'] = createExportWrapper('c2wasm_call_c_function', 1);
var _c2wasm_set_object_prop_function_with_internal_args = Module['_c2wasm_set_object_prop_function_with_internal_args'] = createExportWrapper('c2wasm_set_object_prop_function_with_internal_args', 4);
var _c2wasm_set_object_prop_function = Module['_c2wasm_set_object_prop_function'] = createExportWrapper('c2wasm_set_object_prop_function', 3);
var _malloc = createExportWrapper('malloc', 1);
var _free = createExportWrapper('free', 1);
var _main = Module['_main'] = createExportWrapper('main', 2);
var _fflush = createExportWrapper('fflush', 1);
var _strerror = createExportWrapper('strerror', 1);
var _emscripten_stack_get_end = () => (_emscripten_stack_get_end = wasmExports['emscripten_stack_get_end'])();
var _emscripten_stack_get_base = () => (_emscripten_stack_get_base = wasmExports['emscripten_stack_get_base'])();
var _emscripten_stack_init = () => (_emscripten_stack_init = wasmExports['emscripten_stack_init'])();
var _emscripten_stack_get_free = () => (_emscripten_stack_get_free = wasmExports['emscripten_stack_get_free'])();
var __emscripten_stack_restore = (a0) => (__emscripten_stack_restore = wasmExports['_emscripten_stack_restore'])(a0);
var __emscripten_stack_alloc = (a0) => (__emscripten_stack_alloc = wasmExports['_emscripten_stack_alloc'])(a0);
var _emscripten_stack_get_current = () => (_emscripten_stack_get_current = wasmExports['emscripten_stack_get_current'])();
var dynCall_i = Module['dynCall_i'] = createExportWrapper('dynCall_i', 1);
var dynCall_ii = Module['dynCall_ii'] = createExportWrapper('dynCall_ii', 2);
var dynCall_vi = Module['dynCall_vi'] = createExportWrapper('dynCall_vi', 2);
var dynCall_iii = Module['dynCall_iii'] = createExportWrapper('dynCall_iii', 3);
var dynCall_iiii = Module['dynCall_iiii'] = createExportWrapper('dynCall_iiii', 4);
var dynCall_jiji = Module['dynCall_jiji'] = createExportWrapper('dynCall_jiji', 4);
var dynCall_iidiiii = Module['dynCall_iidiiii'] = createExportWrapper('dynCall_iidiiii', 7);
var dynCall_vii = Module['dynCall_vii'] = createExportWrapper('dynCall_vii', 3);
var _asyncify_start_unwind = createExportWrapper('asyncify_start_unwind', 1);
var _asyncify_stop_unwind = createExportWrapper('asyncify_stop_unwind', 0);
var _asyncify_start_rewind = createExportWrapper('asyncify_start_rewind', 1);
var _asyncify_stop_rewind = createExportWrapper('asyncify_stop_rewind', 0);


// include: postamble.js
// === Auto-generated postamble setup entry stuff ===

var calledRun;

function callMain() {
  assert(runDependencies == 0, 'cannot call main when async dependencies remain! (listen on Module["onRuntimeInitialized"])');
  assert(typeof onPreRuns === 'undefined' || onPreRuns.length == 0, 'cannot call main when preRun functions remain to be called');

  var entryFunction = _main;

  var argc = 0;
  var argv = 0;

  try {

    var ret = entryFunction(argc, argv);

    // if we're not running an evented main loop, it's time to exit
    exitJS(ret, /* implicit = */ true);
    return ret;
  } catch (e) {
    return handleException(e);
  }
}

function stackCheckInit() {
  // This is normally called automatically during __wasm_call_ctors but need to
  // get these values before even running any of the ctors so we call it redundantly
  // here.
  _emscripten_stack_init();
  // TODO(sbc): Move writeStackCookie to native to to avoid this.
  writeStackCookie();
}

function run() {

  if (runDependencies > 0) {
    dependenciesFulfilled = run;
    return;
  }

  stackCheckInit();

  preRun();

  // a preRun added a dependency, run will be called later
  if (runDependencies > 0) {
    dependenciesFulfilled = run;
    return;
  }

  function doRun() {
    // run may have just been called through dependencies being fulfilled just in this very frame,
    // or while the async setStatus time below was happening
    assert(!calledRun);
    calledRun = true;
    Module['calledRun'] = true;

    if (ABORT) return;

    initRuntime();

    preMain();

    Module['onRuntimeInitialized']?.();
    consumedModuleProp('onRuntimeInitialized');

    var noInitialRun = Module['noInitialRun'] || false;
    if (!noInitialRun) callMain();

    postRun();
  }

  if (Module['setStatus']) {
    Module['setStatus']('Running...');
    setTimeout(() => {
      setTimeout(() => Module['setStatus'](''), 1);
      doRun();
    }, 1);
  } else
  {
    doRun();
  }
  checkStackCookie();
}

function checkUnflushedContent() {
  // Compiler settings do not allow exiting the runtime, so flushing
  // the streams is not possible. but in ASSERTIONS mode we check
  // if there was something to flush, and if so tell the user they
  // should request that the runtime be exitable.
  // Normally we would not even include flush() at all, but in ASSERTIONS
  // builds we do so just for this check, and here we see if there is any
  // content to flush, that is, we check if there would have been
  // something a non-ASSERTIONS build would have not seen.
  // How we flush the streams depends on whether we are in SYSCALLS_REQUIRE_FILESYSTEM=0
  // mode (which has its own special function for this; otherwise, all
  // the code is inside libc)
  var oldOut = out;
  var oldErr = err;
  var has = false;
  out = err = (x) => {
    has = true;
  }
  try { // it doesn't matter if it fails
    flush_NO_FILESYSTEM();
  } catch(e) {}
  out = oldOut;
  err = oldErr;
  if (has) {
    warnOnce('stdio streams had content in them that was not flushed. you should set EXIT_RUNTIME to 1 (see the Emscripten FAQ), or make sure to emit a newline when you printf etc.');
    warnOnce('(this may also be due to not including full filesystem support - try building with -sFORCE_FILESYSTEM)');
  }
}

function preInit() {
  if (Module['preInit']) {
    if (typeof Module['preInit'] == 'function') Module['preInit'] = [Module['preInit']];
    while (Module['preInit'].length > 0) {
      Module['preInit'].shift()();
    }
  }
  consumedModuleProp('preInit');
}

preInit();
run();

// end include: postamble.js

