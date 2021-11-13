import React from 'react';
import ReactDOM from 'react-dom';

import Sample from './Sample.js';
import SampleWASM from './Sample.wasm';

const sample = Sample({
    locateFile: () => {
        return SampleWASM;
    },
});

sample.then((core) => {
    try {
        console.log(core.qft(2));
    } catch (e) {
        console.log(e)
    }
});

ReactDOM.render(
    <div>
        <h1>Using WebAssembly with React From Scratch!</h1>
    </div>,
    document.getElementById('root')
);
