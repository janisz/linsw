#!/bin/sh

RED=24

KEY_1=10
KEY_2=22
KEY_3=27

LED_1=23
LED_2=18
LED_3=17

gpio_direction_output $LED_1 1
gpio_direction_output $LED_2 1
gpio_direction_output $LED_3 1
gpio_direction_output $RED 1



gpio_set_value $RED 1
gpio_set_value $LED_1 0
gpio_set_value $LED_2 0
gpio_set_value $LED_3 0

clicked=0
while [ $clicked -eq 0 ]; do
	for i in $KEY_1 $KEY_2 $KEY_3 ; do
		gpio_get_value $i
		if [ $? -eq 0 ]; then			
			clicked=$i
			echo "Clicked " $i
		fi
	done
	if [ $? -eq 0 ]; then			
		clicked=$i
		echo "Clicked " $i
	fi
	if [ $clicked -eg KEY_1 ]; then
		gpio_set_value LED_1 1
	fi
	if [ $clicked -eg KEY_2 ]; then
		gpio_set_value LED_1 1
	fi
	if [ $clicked -eg KEY_3 ]; then
		gpio_set_value LED_1 1
	fi
done
