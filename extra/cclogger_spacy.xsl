<?xml version='1.0' encoding='UTF-8'?>
<!-- 2010-07-15  Michele Tavella <michele.tavella@epfl.ch> -->
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns:lang="Language">
<xsl:output method='html' version='1.0' encoding='UTF-8' indent='yes'/>
<xsl:template match='/'>
	<html>
		<head>
			<title>
				CcLogger - 
				Module: <xsl:value-of select='CcLogger/@module'/> - 
				Timestamp: <xsl:value-of select='CcLogger/@timestamp'/>
			</title>
		</head>
		<style>
			table.logger {
				border-width: 0px;
				border-spacing: 0px;
				border-style: none;
				border-collapse: separate;
				background-color: white;
				width: 100%;
			}
				table.logger th {
				border-width: 0px;
				padding: 1px;
				border-style: none;
				border-color: white;
				-moz-border-radius: 0px 0px 0px 0px;
			}
				table.logger td {
				border-width: 0px;
				padding: 2px;
				border-style: none;
				border-color: white;
				-moz-border-radius: 0px 0px 0px 0px;
			}
			#container {
				margin:	10 auto;
			}
			td {
				padding: 0px;
			}
			html {
				height:	101%;
				font-size: smaller;
			}
			
			body { 
				color: black;
				font-family: sans-serif;
				font-size: 10px;
			}
			xmp {
				color: black;
				margin:	0;
				padding: 0;
				padding-bottom:	10px;
			}
			#left {
				width:		100;
				valign:		top;
			}
			pre {
				width: 99%;
				overflow-x: auto; 
				white-space: pre-wrap; 					/* css-3 */
				white-space: -moz-pre-wrap !important; 	/* Mozilla, since 1999 */
				white-space: -pre-wrap; 				/* Opera 4-6 */
				white-space: -o-pre-wrap; 				/* Opera 7 */
				word-wrap: break-word; 					/* Internet Explorer 5.5+ */
			}
		</style>
		<body bgcolor='white'>
			<center>
				<h2>
					<xsl:value-of select='CcLogger/@module'/> - 
					<xsl:value-of select='CcLogger/@timestamp'/>
				</h2>
			</center>
			<table class="logger">
				<xsl:variable name="empty_string"/>
				<xsl:for-each select='CcLogger/CcLogEntry'>
					<xsl:variable name='LogLevelColor'>
						<xsl:choose>
							<!-- Exception -->
							<xsl:when test="(@level = 'Exception')">
								<xsl:value-of select="'#bf1199'"/>
							</xsl:when>

							<!-- Debug -->
							<xsl:when test="(@level = 'Debug')">
								<xsl:value-of select="'darkgray'"/>
							</xsl:when>

							<!-- Info -->
							<xsl:when test="(@level = 'Info')">
								<xsl:value-of select="'#4abf37'"/>
							</xsl:when>

							<!-- Fine -->
							<xsl:when test="(@level = 'Fine')">
								<xsl:value-of select="'green'"/>
							</xsl:when>

							<!-- Config -->
							<xsl:when test="(@level = 'Config')">
								<xsl:value-of select="'#687de2'"/>
							</xsl:when>

							<!-- Warning -->
							<xsl:when test="(@level = 'Warning')">
								<xsl:value-of select="'orange'"/>
							</xsl:when>

							<!-- Fatal -->
							<xsl:when test="(@level = 'Fatal')">
								<xsl:value-of select="'#ff5a00'"/>
							</xsl:when>

							<!-- Error -->
							<xsl:when test="(@level = 'Error')">
								<xsl:value-of select="'#ff8200'"/>
							</xsl:when>


							<!-- Unknown -->
							<xsl:otherwise>
								<xsl:value-of select="'white'"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<tr bgcolor='{$LogLevelColor}'>
						<td width='100px' height='26px'><b><xsl:value-of select='@level'/></b></td>
						<td></td>
					</tr>
					<tr>
						<td width='100px' valign='top'><b>Timestamp</b></td>
						<td><xsl:value-of select='@timestamp'/></td>
					</tr>
					<tr>
						<td width='100px' valign='top'><b>Caller</b></td>
						<td><xsl:value-of select='caller'/></td>
					</tr>
					<tr>
						<td width='100px' valign='top'><b>File</b></td>
						<td><xsl:value-of select='file'/>:<xsl:value-of select='line'/></td>
					</tr>
					<tr>
						<td width='100px' valign='top'><b>Message</b></td>
						<td><xsl:value-of select='message'/></td>
					</tr>
					<tr>
						<td height='10px'></td>
					</tr>
				</xsl:for-each>
			</table>
		</body>
	</html>
</xsl:template>
</xsl:stylesheet>
