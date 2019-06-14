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

use qompose::error::*;
use qompose::term::esc;
use qompose::term::key;
use qompose::term::mode::RawModeHandle;
use qompose::term::util;
use std::io::{self, Write};

fn paint() -> Result<()> {
    let (lines, _) = util::get_terminal_size()?;

    let mut buf: Vec<u8> = vec![];

    esc::apply_write(&mut buf, esc::hide_cursor())?;
    esc::apply_write(&mut buf, esc::erase_display(esc::EraseDisplayMode::All))?;
    esc::apply_write(&mut buf, esc::move_cursor(None, None))?;

    for l in 0..lines {
        write!(buf, "~")?;
        if l < lines - 1 {
            write!(buf, "\r\n")?;
        }
    }
    esc::apply_write(&mut buf, esc::move_cursor(None, None))?;
    esc::apply_write(&mut buf, esc::show_cursor())?;

    io::stdout().write_all(buf.as_slice())?;
    io::stdout().flush()?;
    Ok(())
}

fn run() -> Result<()> {
    let _raw_mode_handle = RawModeHandle::new()?;

    loop {
        paint()?;

        if let Some(c) = key::read_key()? {
            if c == key::get_ctrl_key('q')? {
                esc::apply(esc::erase_display(esc::EraseDisplayMode::All))?;
                esc::apply(esc::move_cursor(None, None))?;
                esc::flush()?;
                break;
            }
        }
    }

    Ok(())
}

fn main() {
    if let Err(e) = run() {
        let mut stderr = io::stderr();
        writeln!(stderr, "{}", e).expect("Error writing to STDERR");
    }
}
