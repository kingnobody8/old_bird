//Started script log at 05/02/16 20:45:39

getBody(2).select();
getBody(2).setPosition(19.3517,27.882);
{
	fixture _rube_redoFixture = getFixture(2);
	_rube_redoFixture.setVertex(0,-6.05207,-10.5);
	_rube_redoFixture.setVertex(1,6.05205,-10.5);
	_rube_redoFixture.setVertex(2,6.05205,10.5001);
	_rube_redoFixture.setVertex(3,-6.05207,10.5001);
}
getBody(2).deselect();
getBody(13).select();
getBody(13).deselect();
getFixture(13).delete();
getBody(13).delete();
getBody(6).select();
getBody(6).deselect();
getFixture(6).delete();
getBody(6).delete();
getBody(2).select();
{
	fixture _rube_redoFixture = getFixture(2);
	_rube_redoFixture.setVertex(0,-1.30354,-2.26156);
	_rube_redoFixture.setVertex(1,1.30353,-2.26156);
	_rube_redoFixture.setVertex(2,1.30353,2.26159);
	_rube_redoFixture.setVertex(3,-1.30354,2.26159);
}
getFixture(2).select();
getFixture(2).setFilterCategoryBits(0);
getFixture(2).deselect();
getBody(2).setType(0);
getBody(2).deselect();
getBody(2).select();
getBody(2).deselect();
getBody(2).select();
{
	fixture _rube_redoFixture = getFixture(2);
	_rube_redoFixture.setVertex(0,-1.40024,-2.42933);
	_rube_redoFixture.setVertex(1,1.40024,-2.42933);
	_rube_redoFixture.setVertex(2,1.40024,2.42936);
	_rube_redoFixture.setVertex(3,-1.40024,2.42936);
}
