<?xml version="1.0"?>
<xsl:stylesheet version="1.0" 
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
 xmlns:fo="http://www.w3.org/1999/XSL/Format">
 
	<xsl:param name="eventType" select="'all'"/>
	<xsl:param name="nameSpace" select="'any'"/>
	<xsl:param name="specificFile" select="''"/>
	
	<xsl:template match="/">
		<html>
				<head>
						<Title>Log file viewer</Title>
				</head>
				<body>
						<font face="Arial" size="5" color="#2060AA">
								<b><u>Run-Time log</u></b>
						</font>
						<br/>
								<xsl:apply-templates select="RunTimeLog/LogHeader"/>
						<br/>
						<br/>
						<b>
								<font face="Arial" size="3" color="#AAAAAA">
										Details of selected log entries:
								</font>
						</b>
						<br/>
						<font face="Arial" size="2" color="#AAAAAA">
								This output is desplaying events of type
								<b>
										'<xsl:copy-of select="$eventType"/>'
								</b>
								That originated from
								<b>
										<xsl:copy-of select="$specificFile"/>
								</b>
								within the nameSpace
								<b>
										'<xsl:copy-of select="$nameSpace"/>'
								</b>
						</font>
						<br/>
						<br/>
						
						<table border="1" width="100%" cellspacing="0" cellpadding="0" bordercolorlight="#000000"
						 bordercolordark="#ffffff" bordercolor="#000000">
								<tr>
										<td width="3%" bgcolor="#000000"><font size="2" face="Arial" color="#ffffff"><b><center>#</center>
											</b></font></td>
										<td width="20%" bgcolor="#000000"><font size="2" face="Arial" color="#ffffff"><b><center>Time
											</center></b></font></td>
										<td width="23%" bgcolor="#000000"><font size="2" face="Arial" color="#ffffff"><b><center>File
											</center></b></font></td>
										<td width="50%" bgcolor="#000000"><font size="2" face="Arial" color="#ffffff"><b><center>Function
											</center></b></font></td>
										<td width="4%" bgcolor="#000000"><font size="2" face="Arial" color="#ffffff"><b><center>Line
											</center></b></font></td>
								</tr>
								
								<xsl:apply-templates select="RunTimeLog/LogEvent"/>
						</table>
				</body>
		</html>
	</xsl:template>
	
	<xsl:template match="LogHeader">
			<br/>
			<b>
					<font face="Arial" size="3" color="#AAAAAA">
							Log file header information:
					</font>
			</b>
			<br/>
			<font face="Arial" size="2" color="#AAAAAA">
					Log file description:
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:value-of select="Description"/>
					</font>
			</i>
			<br/>
			<font face="Arial" size="2" color="#AAAAAA">
					Operating system:
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:value-of select="Session/Configuration/OSType"/>
					</font>
			</i>
			<br/>
			<font face="Arial" size="2" color="#AAAAAA">
					Number of processors installed:
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:value-of select="Session/Configuration/NumberOfProcessors"/>
					</font>
			</i>
			<br/>
			<font face="Arial" size="2" color="#AAAAAA">
					Processor type:
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:value-of select="Session/Configuration/ProcessorType"/>
					</font>
			</i>
			<br/>
			<font face="Arial" size="2" color="#AAAAAA">
					Session started at
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:value-of select="Session/Started/Time"/>
					</font>
			</i>
			<font face="Arial" size="2" color="#AAAAAA">
					on
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:value-of select="Session/Started/Date"/>
					</font>
			</i>
			<br/>
			<font face="Arial" size="2" color="#AAAAAA">
					Logged events number:
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:value-of select="Session/LoggedEventsNum"/>
					</font>
			</i>
			<br/>
			<font face="Arial" size="2" color="#AAAAAA">
					Total logged events:
			</font>
			<i>
					<font face="Arial" size="2" color="#808080">
							<xsl:copy-of select="count(../LogEvent)"/>
					</font>
			</i>
  </xsl:template>
   
	<xsl:template match="LogEvent">
	
		<xsl:if test="$specificFile='' or $specificFile=File">
			<xsl:choose>
				<xsl:when test="Type='3'">
					<xsl:if test="$eventType='all' or $eventType='tracking' or $eventType='Comment'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#80FF80" valign="middle" align="center">

								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>

							</tr>
							<tr bgcolor="#AAFFAA">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
				<xsl:when test="Type='1'">
					<xsl:if test="$eventType='all' or $eventType='Unknown'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#EEEEEE" valign="middle" align="center">
							
								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>
								
							</tr>
							<tr bgcolor="#AAAAAA">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
				<xsl:when test="Type='6'">
					<xsl:if test="$eventType='all' or $eventType='failures' or $eventType='Error'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#FF8080" valign="middle" align="center">
							
								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>
								
							</tr>
							<tr bgcolor="#FFAAAA">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
				<xsl:when test="Type='7'">
					<xsl:if test="$eventType='all' or $eventType='failures' or $eventType='Fatal Error'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#FF0000" valign="middle" align="center">
							
								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>
								
							</tr>
							<tr bgcolor="#FFAAAA">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
				<xsl:when test="Type='5'">
					<xsl:if test="$eventType='all' or $eventType='failures' or $eventType='Warning'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#FFCC00" valign="middle" align="center">

								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>
								
							</tr>
							<tr bgcolor="#FFFF00">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
				<xsl:when test="Type='4'">
					<xsl:if test="$eventType='all' or $eventType='tracking' or $eventType='Event'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#8080FF" valign="middle" align="center">
							
								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>
								
							</tr>
							<tr bgcolor="#AAAAFF">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
				<xsl:when test="Type='8'">
					<xsl:if test="$eventType='all' or $eventType='Debug'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#FFFF80" valign="middle" align="center">
							
								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>
								
							</tr>
							<tr bgcolor="#FFFF99">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
				<xsl:when test="Type='2'">
					<xsl:if test="$eventType='all' or $eventType='tracking' or $eventType='Game Message'">
						<xsl:if test="$nameSpace='any' or $nameSpace=NameSpace">
							<tr bgcolor="#FF8020" valign="middle" align="center">
							
								<td>				
									<font size="2" face="Arial" color="#202020">
										<center>
											<xsl:value-of select="@id"/>
										</center>
									</font>
								</td>
								
								<xsl:apply-templates select="TimeIndex"/>
								<xsl:apply-templates select="File"/>
								<xsl:apply-templates select="Function"/>
								<xsl:apply-templates select="LineNumber"/>
								
							</tr>
							<tr bgcolor="#FFAA80">
								<td></td>
								<xsl:apply-templates select="Message"/>
							</tr>
						</xsl:if>
					</xsl:if>
					
				</xsl:when>
			</xsl:choose>
		</xsl:if>

	</xsl:template>

	<xsl:template match="Message">
			<td colspan="4">
					<font size="2" face="Courier New" color="#404040">
							<center>
									<xsl:apply-templates/>
							</center>
					</font>
			</td>
	</xsl:template>
	
	<xsl:template match="LineNumber">
			<td>
					<font size="2" face="Courier New" color="#404040">
							<center>
									<xsl:apply-templates/>
							</center>
					</font>
			</td>
	</xsl:template>
	
	<xsl:template match="File">
			<td>
					<font size="2" face="Courier New" color="#404040">
							<center>
									<xsl:apply-templates/>
							</center>
					</font>
			</td>
	</xsl:template>
	
	<xsl:template match="Function">
			<td>
					<font size="2" face="Courier New" color="#404040">
							<center>
									<xsl:apply-templates/>
							</center>
					</font>
			</td>
	</xsl:template>
	
	<xsl:template match="TimeIndex">
			<td>
					<font size="2" face="Courier New" color="#404040">
							<center>
									<xsl:apply-templates/>
							</center>
					</font>
			</td>
	</xsl:template>

</xsl:stylesheet>