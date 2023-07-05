# BOM

Here is a preliminary BOM for my version of the CW Messenger.  The biggest differences from the project in the book are that I'm using a 2.8" display rather than a 2.4" one and (2) my case is off-the-shelf rather than 3D printed and is a different size.  The total price for the project, sourced from Mouser is about $81.  You can save some money with alternate sourcing.  I've indicated volume discounts for a few components which may be attractive if you plan to build more projects.

| Item    | Description | Part # | Price | Comments |
| -------- | ------- | ------- | -------: | ------- |
| U1 | ESP32-WROOM-32 | [ESP32-DevKitC-32E](https://www.mouser.com/ProductDetail/356-ESP32-DEVKITC32E) | $10.00 |  |
| U2 | TFT LCD Display | [DFR0665](https://www.mouser.com/ProductDetail/426-DFR0665) | $24.90 | 2.8" LCD; book uses 2.4" |
| U3 | Voltage Regulator | [L7805CV](https://www.mouser.com/ProductDetail/511-L7805CV) | $0.69 |  |
| U4 | Optocoupler | [4N25](https://www.mouser.com/ProductDetail/78-4N25) | $0.60 | 10 @ $3.68 |
| U5 | Rotary Encoder | [PEC11R-4230K-S0024](https://www.mouser.com/ProductDetail/652-PEC11R4230KS0024) | $2.16 |  |
| Q1 | NPN Transistor | [2N3904](https://www.mouser.com/ProductDetail/637-2N3904) | $0.42 | 10 @ $1.40 |
| D1 | Reverse Polarity Diode | [BAT48](https://www.mouser.com/ProductDetail/511-BAT48) | $0.47 | SCHR msg #21252. I used a Schottky diode |
| R1 | Potentiameter | [PDB181-B225K-503B](https://www.mouser.com/ProductDetail/652-PDB181-B225K503B) | $1.52 | select as needed by buzzer; 50k here, text suggests 75k for loud buzzer; SCHR msg #21252 indicates `10k?` |
| C1, C2 | 100 uF | [RGA101M1EBK-0611G](https://www.mouser.com/ProductDetail/Lelon/RGA101M1EBK-0611G?qs=pNDdEcSXiOD7yLUz3NUJrA%3D%3D) | $0.12 | value from SCHR msg #21252 |
| C3 | 10 uF | [667-EEA-GA1E100H](https://www.mouser.com/ProductDetail/Panasonic/EEA-GA1E100H?qs=0h1gzos03f1yprFRE2oW8A%3D%3D) | $0.28 | value from SCHR msg #21252; 10 @ $1.27 |
| S1, S2, S3 | Pushbutton Switch | [PS1024AT-BLK](https://www.mouser.com/ProductDetail/612-PS1024ABLK) | $1.42 |  |
| S4 | Toggle Switch | [2M1-SP1-T1-B1-M1Q](https://www.mouser.com/ProductDetail/691-2M1-SP1-T1B1M1QE) | $2.33 |  |
| J1, J2 | Jack | [SJ5-43502PM](https://www.mouser.com/ProductDetail/490-SJ5-43502PM) | $2.50 | 4 position, wire per schematic |
|  | Buzzer | [CMT-9605-85T](https://www.mouser.com/ProductDetail/490-CMT-9605-85T) | $0.85 |  |
|  | Knobs | [1227-L](https://www.mouser.com/ProductDetail/5164-1227-L) | $1.69 | need 2 |
|  | DC Power Jack | [PJ-005A](https://www.mouser.com/ProductDetail/490-PJ-005A) | $2.54 |  |
|  | Protoboard | [PAD1](https://www.mouser.com/ProductDetail/854-PAD1) | $3.94 |  |
|  | Case | [CM6-300](https://www.mouser.com/ProductDetail/616-63999-510-039) | $17.25 |  |
|  | Socket | [1-2199298-1](https://www.mouser.com/ProductDetail/571-1-2199298-1) | $0.26 | 10 @ $1.69 |
|  |  |  |  |  |

## Notes

* Parts # reflect manufacturer's number as listed by Mouser.
* Pricing as of 7/4/2023.
* Other size cases may work as well.  I may make mine a handheld using with CM5-200 or CM5-125.
* Other items needed: wire, display and protoboard mounting hardware.
