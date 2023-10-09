#![no_std]
#![no_main]

use panic_halt as _;
use arduino_hal::spi;
use embedded_nal::{IpAddr, Ipv4Addr, SocketAddr};

#[arduino_hal::entry]
fn main() -> ! {
    let dp = arduino_hal::Peripherals::take().unwrap();
    let pins = arduino_hal::pins!(dp);
    let select = pins.d10.into_output();

    let (spi, _) = arduino_hal::Spi::new(
        dp.SPI,
        pins.d13.into_output(),
        pins.d11.into_output(),
        pins.d12.into_pull_up_input(),
        select,
        spi::Settings::default(),
    );

    let mut device = w5500::UninitializedDevice::new(w5500::bus::FourWire::new(spi, select))
        .initialize_manual(
            w5500::MacAddress::new(0, 1, 2, 3, 4, 5),
            w5500::net::Ipv4Addr::new(192, 168, 0, 190),
            w5500::Mode::default()
        ).unwrap();

    let mut socket = device.socket().unwrap();

    device.connect(&mut socket, SocketAddr::new(IpAddr::V4(w5500::net::IpAddr::new(192, 168, 0, 158)), 7632)).unwrap();

    nb::block!(device.send(&mut socket, &[104, 101, 108, 108, 111, 10]));

    device.close(socket);

    let (spi_bus, inactive_device) = device.deactivate();
}
