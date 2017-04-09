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
use term::STDIN_FILENO;
use termios::{self, Termios};

pub struct RawModeHandle {
    original_termios: Termios,
}

impl RawModeHandle {
    pub fn new() -> Result<RawModeHandle> {
        let mut raw = Termios::from_fd(STDIN_FILENO)?;
        let original = raw.clone();

        // Turn off break, '\r' translation, parity check, strip char, and software
        // flow control (Ctrl-{S,Q}).
        raw.c_iflag &= !(termios::BRKINT | termios::ICRNL | termios::INPCK | termios::ISTRIP |
                         termios::IXON);
        // Turn off all output post-processing.
        raw.c_oflag &= !(termios::OPOST);
        // Set 8 bit chars explicitly.
        raw.c_cflag |= termios::CS8;
        // Turn off character echoing, canonical mode (wait for 'enter' to read),
        // Ctrl-V (which sends the next character literally), and some signals.
        raw.c_lflag &= !(termios::ECHO | termios::ICANON | termios::IEXTEN | termios::ISIG);

        // Return each byte from read(), or timeout on 0 bytes.
        raw.c_cc[termios::VMIN] = 0;
        // Timeout read() after 100ms of no bytes.
        raw.c_cc[termios::VTIME] = 1;

        termios::tcsetattr(STDIN_FILENO, termios::TCSAFLUSH, &raw)?;

        Ok(RawModeHandle { original_termios: original })
    }
}

impl Drop for RawModeHandle {
    fn drop(&mut self) {
        termios::tcsetattr(STDIN_FILENO, termios::TCSAFLUSH, &self.original_termios).unwrap();
    }
}
