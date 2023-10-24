# -*- coding: utf-8 -*-
"""
Created on Wed Jun 14 18:18:54 2023

@author: SEYDINA
"""

from sensapex import UMP
import time 
ump = UMP.get_ump()
dev_ids = ump.list_devices()
print (dev_ids)

manipulator = ump.get_device(1)
manipulator.goto_pos((7320.5, 22201.5, 6662.1), speed=2000)

time.sleep(20)

manipulator.goto_pos((-105.5000, 16346.0000, 223.6000), speed=2000)


#  X Y Z 