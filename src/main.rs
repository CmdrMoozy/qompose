// Copyright 2013 Axel Rasmussen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#[macro_use]
extern crate error_chain;
extern crate qompose;
extern crate termios;

use qompose::error::*;
use qompose::term::mode::RawModeHandle;
use std::io::{self, Read, Write};

fn read_char_stdin() -> Result<char> {
    Ok(char::from(match io::stdin().bytes().next() {
        Some(c) => c?,
        None => bail!("STDIN returned EOF"),
    }))
}

fn run() -> Result<()> {
    let _raw_mode_handle = RawModeHandle::new()?;

    loop {
        let c = read_char_stdin()?;
        if c == 'q' {
            break;
        }
    }

    Ok(())
}

fn main() {
    if let Err(e) = run() {
        use error_chain::ChainedError;
        let mut stderr = io::stderr();
        writeln!(stderr, "{}", e.display()).expect("Error writing to STDERR");
    }
}
