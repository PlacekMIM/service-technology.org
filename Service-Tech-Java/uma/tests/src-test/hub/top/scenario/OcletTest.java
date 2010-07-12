package hub.top.scenario;

import java.util.LinkedList;

public class OcletTest {
  
  public String lastTest = "";
  public int testNum = 0;
  public int testFail = 0;
  
  public OcletTest(String name) {
    super();
  }
  

  private static final String RESULT_FAIL = "[failed]";
  private static final String RESULT_OK = "[ok]";
  
  public void assertTrue(boolean result) {
    System.out.println(lastTest+": "+result+", expected: true "+(result ? RESULT_OK : RESULT_FAIL));
    if (!result) testFail++;
  }
  
  public void assertFalse(boolean result) {
    System.out.println(lastTest+": "+result+", expected: false "+(!result ? RESULT_OK : RESULT_FAIL));
    if (result) testFail++;
  }
  
  public void run () {
    testOcletPrefix_fail();
    testOcletCycle_fail();
    testOcletAll_yes();
  }

  public static void main(String[] args) {
    OcletTest t = new OcletTest("Oclets");
    t.run();
  }
  
  public void testOcletPrefix_fail () {
    
    lastTest = "Detect wrong prefix in oclet";
    
    Oclet o = new Oclet(false);
    o.addPlace("p1a", true); o.addPlace("p1b", true); o.addPlace("p1c", true);
    o.addPlace("p2a", true); o.addPlace("p2b", true); o.addPlace("p2c", true);
    
    o.addPlace("p3a", false); o.addPlace("p3b", false); o.addPlace("p3c", false);
    
    o.addTransition("e1a", true);
      o.addArc( "p1a", "e1a" ); o.addArc( "p1b", "e1a" );
      o.addArc( "e1a", "p2a" );
      
    o.addTransition("e1b", true);
      o.addArc( "p1c", "e1b" );
      o.addArc( "e1b", "p2b" ); o.addArc("e1b", "p2c" );
      
    o.addTransition("e2a", false);
      o.addArc( "p2a", "e2a" ); o.addArc( "p2b", "e2a" );
      o.addArc( "e2a", "p3a" );
      
    o.addTransition("e2b", false);
      o.addArc( "p2c", "e2b" );
      o.addArc( "e2b", "p3b" ); o.addArc( "e2b", "p3c" );

    o.addTransition("e3", false);
      o.addArc( "p3c", "e3" );
      o.addArc( "e3", "p1b" );
    
    assertTrue(o.isCausalNet() == true && o.historyIsPrefix() == false);
  }
  
  public void testOcletCycle_fail () {
    
    lastTest = "Detect cycle in oclet";
    
    Oclet o = new Oclet(false);
    o.addPlace("p1a", true); o.addPlace("p1b", true); o.addPlace("p1c", true);
    o.addPlace("p2a", true); o.addPlace("p2b", true); o.addPlace("p2c", true);
    
    o.addPlace("p3a", false); o.addPlace("p3b", false); o.addPlace("p3c", false);
    
    o.addTransition("e1a", true);
      o.addArc( "p1a", "e1a" ); o.addArc( "p1b", "e1a" );
      o.addArc( "e1a", "p2a" );
      
    o.addTransition("e1b", true);
      o.addArc( "p1c", "e1b" );
      o.addArc( "e1b", "p2b" ); o.addArc("e1b", "p2c" );
      
    o.addTransition("e2a", false);
      o.addArc( "p2a", "e2a" ); o.addArc( "p2b", "e2a" );
      o.addArc( "e2a", "p3a" );
      
    o.addTransition("e2b", false);
      o.addArc( "p2c", "e2b" );
      o.addArc( "e2b", "p3b" ); o.addArc( "e2b", "p3c" );

    o.addTransition("e3", false);
      o.addArc( "p3a", "e3" );
      o.addArc( "e3", "p1b" );
      
    assertFalse( o.isCausalNet() );
  }
  
  public void testOcletAll_yes () {
    
    lastTest = "Check valid oclet";
    
    Oclet o = new Oclet(false);
    o.addPlace("p1a", true); o.addPlace("p1b", true); o.addPlace("p1c", true);
    o.addPlace("p2a", true); o.addPlace("p2b", true); o.addPlace("p2c", true);
    
    o.addPlace("p3a", false); o.addPlace("p3b", false); o.addPlace("p3c", false);
    o.addPlace("p4", false);
    
    o.addTransition("e1a", true);
      o.addArc( "p1a", "e1a" ); o.addArc( "p1b", "e1a" );
      o.addArc( "e1a", "p2a" );
      
    o.addTransition("e1b", true);
      o.addArc( "p1c", "e1b" );
      o.addArc( "e1b", "p2b" ); o.addArc("e1b", "p2c" );
      
    o.addTransition("e2a", false);
      o.addArc( "p2a", "e2a" ); o.addArc( "p2b", "e2a" );
      o.addArc( "e2a", "p3a" );
      
    o.addTransition("e2b", false);
      o.addArc( "p2c", "e2b" );
      o.addArc( "e2b", "p3b" ); o.addArc( "e2b", "p3c" );

    o.addTransition("e3", false);
      o.addArc( "p3a", "e3" );
      o.addArc( "e3", "p4" );
      
    assertTrue( o.isValidOclet() );
  }  
}