import os

license_line = """/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */

"""

for root, dirs, files in os.walk('.'):
	if '.svn' in dirs:
		dirs.remove( '.svn' ) # don't look in here :)
	
	for fn in files:
		if fn[-2:] != '.h' and fn[-2:] != '.c' and fn[-2:] != '.m':
			continue
		
		tfn = root + '/' + fn
		
		f = open( tfn, "r" )
		ftmp = open( tfn + '.tmp', "w" )
		
		lnum = 0
		skip = False
		
		for line in f.readlines( ):
			lnum += 1
			
			if lnum == 1 and line.strip() == '/* INSERT HEADER HERE */':
				line = license_line
			elif lnum == 1:
				print "WARNING: file",tfn,"didn't have a license header, skipping."
				skip = True
			
			ftmp.write( line )
		
		ftmp.close( )
		f.close( )
		
		if skip:
			os.unlink( tfn + '.tmp' )
		else:
			os.unlink( tfn )
			os.rename( tfn + '.tmp', tfn )