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

use error::*;
use std::io;

/// Given a character c, return the character we would read on STDIN when the
/// user types Ctrl+(c).
///
/// This function is only defined for ASCII characters. If c is not an ASCII
/// character, an error is returned instead.
pub fn get_ctrl_key(c: char) -> Result<char> {
    if c.len_utf8() != 1 {
        bail!("Cannot compute Ctrl+[char] sequence for non-ASCII characters");
    }
    let mut bytes = [0; 4];
    c.encode_utf8(&mut bytes);
    Ok(char::from(bytes[0] & 0x1F))
}

/// Read a single keystroke from STDIN. If an I/O error occurs, return an
/// error. If our read timed out without detecting any keystrokes, Ok(None) is
/// returned instead.
pub fn read_key() -> Result<Option<char>> {
    use std::io::Read;
    let byte: Option<u8> = match io::stdin().bytes().next() {
        Some(b) => Some(b?),
        None => None,
    };
    Ok(byte.map(|b| char::from(b)))
}
