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
use std::io::{self, Write};

fn escape_sequence(seq: &str) -> Vec<u8> {
    [0x1b_u8, 0x5b_u8].iter().cloned().chain(seq.bytes()).collect()
}

pub enum EraseDisplayMode {
    PositionToEnd,
    StartToPosition,
    All,
}

pub fn erase_display(mode: EraseDisplayMode) -> Vec<u8> {
    match mode {
        EraseDisplayMode::PositionToEnd => escape_sequence("0J"),
        EraseDisplayMode::StartToPosition => escape_sequence("1J"),
        EraseDisplayMode::All => escape_sequence("2J"),
    }
}

pub fn hide_cursor() -> Vec<u8> { escape_sequence("?25l") }

pub fn show_cursor() -> Vec<u8> { escape_sequence("?25h") }

pub fn cursor_position_report() -> Vec<u8> { escape_sequence("6n") }

pub fn move_cursor(line: Option<usize>, column: Option<usize>) -> Vec<u8> {
    escape_sequence(format!("{};{}H", line.unwrap_or(1), column.unwrap_or(1)).as_str())
}

pub fn move_cursor_down(lines: usize) -> Vec<u8> { escape_sequence(format!("{}B", lines).as_str()) }

pub fn move_cursor_forward(columns: usize) -> Vec<u8> {
    escape_sequence(format!("{}C", columns).as_str())
}

pub fn apply_write<W: Write>(w: &mut W, seq: Vec<u8>) -> Result<()> {
    Ok(w.write_all(seq.as_slice())?)
}

pub fn apply(seq: Vec<u8>) -> Result<()> { apply_write(&mut io::stdout(), seq) }

pub fn flush() -> Result<()> {
    let mut stdout = io::stdout();
    Ok(stdout.flush()?)
}
