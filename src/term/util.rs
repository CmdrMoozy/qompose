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

use crate::error::*;
use crate::term::esc;
use failure::bail;
use std::io::{self, Read};
use std::iter::FromIterator;

const MAX_CURSOR_POSITION_REPORT_LENGTH: usize = 1024;

/// Retrieve the current cursor position as a (1-indexed) (line, column) tuple.
pub fn get_cursor_position() -> Result<(usize, usize)> {
    esc::apply(esc::cursor_position_report())?;
    esc::flush()?;

    // The terminal will respond with a cursor position report, which is itself an
    // escape sequence, formatted like: ESC [ Pn ; Pn R where the parameters are
    // line and column, respectively. So, read until we see the 'R' character.
    let mut split_idx: usize = 0;
    let mut chars: Vec<char> = vec![];
    loop {
        let byte = io::stdin().bytes().next();
        if byte.is_none() {
            bail!("Read EOF while waiting for cursor position report");
        }
        let c = char::from(byte.unwrap()?);
        chars.push(c);

        if c == ';' {
            split_idx = chars.len() - 1;
        }
        if c == 'R' {
            break;
        }
        if chars.len() > MAX_CURSOR_POSITION_REPORT_LENGTH {
            bail!("Cursor position report exceeded maximum allowed length");
        }
    }

    if split_idx == 0
        || chars.len() < 6
        || chars[0] != char::from(0x1b_u8)
        || chars[1] != '['
        || chars[chars.len() - 1] != 'R'
    {
        bail!("Cursor position report format is invalid");
    }

    let line: usize = String::from_iter(chars[2..split_idx].iter().cloned()).parse()?;
    let col: usize =
        String::from_iter(chars[split_idx + 1..chars.len() - 1].iter().cloned()).parse()?;
    Ok((line, col))
}

/// Retrieve the full size of the terminal as a (lines, columns) tuple.
pub fn get_terminal_size() -> Result<(usize, usize)> {
    // First, move the cursor all the way to the bottom right of the terminal. We
    // move X and Y separately, because unlike `move_cursor` these functions have
    // well defined behavior when we move off-screen (namely, they stop at the edge
    // of the terminal).
    esc::apply(esc::move_cursor_down(i32::max_value() as usize))?;
    esc::apply(esc::move_cursor_forward(i32::max_value() as usize))?;
    // Get the cursor's position, and we have the terminal size.
    get_cursor_position()
}
