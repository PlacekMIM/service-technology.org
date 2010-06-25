/*****************************************************************************\
 * Copyright (c) 2010. All rights reserved. Dirk Fahland. AGPL3.0
 * 
 * ServiceTechnology.org - Petri Net API/Java
 * 
 * This program and the accompanying materials are made available under
 * the terms of the GNU Affero General Public License Version 3 or later,
 * which accompanies this distribution, and is available at 
 * http://www.gnu.org/licenses/agpl.txt
 * 
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 * 
\*****************************************************************************/

package hub.top.petrinet;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;

import org.antlr.runtime.ANTLRFileStream;
import org.antlr.runtime.CommonTokenStream;
import org.antlr.runtime.EarlyExitException;
import org.antlr.runtime.MismatchedTokenException;
import org.antlr.runtime.RecognitionException;

/**
 * 
 * Provides front-end methods for reading and writing Petri nets.
 * 
 * @author Dirk Fahland
 */
public class PetriNetIO {
  
  /**
   * Read Petri net from a text-file.
   * 
   * @param fileName
   * @return the Petri net written in <code>fileName</code>
   * @throws IOException
   */
  public static PetriNet readNetFromFile(String fileName) throws IOException {
    
    String ext = fileName.substring(fileName.lastIndexOf('.')+1);
    
    if ("lola".equals(ext)) {
    
      LoLALexer lex = new LoLALexer(new ANTLRFileStream(fileName));
      CommonTokenStream tokens = new CommonTokenStream(lex);
  
      LoLAParser parser = new LoLAParser(tokens);
  
      try {
        PetriNet result = parser.net();
        return result;
          
      } catch (RecognitionException e)  {
        if (e instanceof EarlyExitException) {
          EarlyExitException e2 = (EarlyExitException)e;
          System.err.println("failed parsing "+fileName);
          System.err.println("found unexpected '"+e2.token.getText()+"' in "
              +"line "+e2.line+ " at column "+e2.charPositionInLine);
          System.exit(1);
        } else {
          e.printStackTrace();
        }
      }
      
    } else if ("owfn".equals(ext)) {

      OWFNLexer lex = new OWFNLexer(new ANTLRFileStream(fileName));
      CommonTokenStream tokens = new CommonTokenStream(lex);
  
      OWFNParser parser = new OWFNParser(tokens);
  
      try {
        PetriNet result = parser.net();
        return result;
          
      } catch (RecognitionException e)  {
          if (e instanceof EarlyExitException) {
            EarlyExitException e2 = (EarlyExitException)e;
            System.err.println("failed parsing "+fileName);
            System.err.println("found unexpected '"+e2.token.getText()+"' in "
                +"line "+e2.line+ " at column "+(e2.charPositionInLine+1));
            System.exit(1);
          } else if (e instanceof MismatchedTokenException) {
            MismatchedTokenException e2 = (MismatchedTokenException)e;
            System.err.println("failed parsing "+fileName);
            System.err.println("line "+e2.line+":"+(e2.charPositionInLine+1)+" found unexpected "+e2.token.getText());
            System.exit(1);
          } else {
            e.printStackTrace();
          }
      }
    }
    
    return null;
  }
  
  public static final int FORMAT_DOT = 1;
  public static final int FORMAT_LOLA = 2;
  public static final int FORMAT_OWFN = 3;
  
  public static String toLoLA(String ident) {
    String result = ident.replace(' ', '_');
    result = result.replace('(', '_');
    result = result.replace(')', '_');
    result = result.replace('[', '_');
    result = result.replace(']', '_');
    result = result.replace('=', '_');
    return result;
  }
  
  public static String toLoLA(PetriNet net) {
    StringBuilder b = new StringBuilder();
    
    b.append("{\n");
    b.append("  input file:\n");
    b.append("  invocation:\n");
    b.append("  net size:     "+net.getInfo()+"\n");
    b.append("}\n\n");
    
    // ---------------------- places ------------------------
    b.append("PLACE");
      b.append("    ");
      Iterator<Place> place = net.getPlaces().iterator();
      while (place.hasNext()) {
        b.append(" "+toLoLA(place.next().getUniqueIdentifier()));
        if (place.hasNext()) b.append(",");
      }
      b.append(";\n");
    b.append("\n");
    
    // ---------------------- markings ------------------------
    b.append("MARKING\n");
    b.append("  ");
    boolean firstPlace = true;
    for (Place p : net.getPlaces()) {
      if (p.getTokens() > 0) {
        if (!firstPlace) b.append(",");
        b.append(" "+toLoLA(p.getUniqueIdentifier())+":"+p.getTokens());
        firstPlace = false;
      }
    }
    b.append(";\n\n");
    
    // ---------------------- transitions ------------------------
    for (Transition t : net.getTransitions()) {
      b.append("TRANSITION "+toLoLA(t.getUniqueIdentifier())+"\n");
        // pre-places of the transition
        b.append("  CONSUME");
        place = t.getPreSet().iterator();
        while (place.hasNext()) {
          b.append(" "+toLoLA(place.next().getUniqueIdentifier())+":1");
          if (place.hasNext()) b.append(",");
        }
        b.append(";\n");
        // post-places of the transition
        b.append("  PRODUCE");
        place = t.getPostSet().iterator();
        while (place.hasNext()) {
          b.append(" "+toLoLA(place.next().getUniqueIdentifier())+":1");
          if (place.hasNext()) b.append(",");
        }
        b.append(";\n");
      b.append("\n");
    }
    return b.toString();
  }
  
  public static String toOWFN(PetriNet net) {
    StringBuilder b = new StringBuilder();
    
    b.append("{\n");
    b.append("  input file:\n");
    b.append("  invocation:\n");
    b.append("  net size:     "+net.getInfo()+"\n");
    b.append("}\n\n");
    
    // ---------------------- places ------------------------
    b.append("PLACE");
      // print roles
      if (net.getRoles().size() > 0) {
        b.append("  ROLES");
        Iterator<String> role = net.getRoles().iterator();
        while (role.hasNext()) {
          b.append(" "+toLoLA(role.next()));
          if (role.hasNext()) b.append(",");
        }
        b.append(";\n");
      }
      // print internal places
      b.append("  INTERNAL\n");
        b.append("    ");
        Iterator<Place> place = net.getPlaces().iterator();
        while (place.hasNext()) {
          b.append(" "+toLoLA(place.next().getUniqueIdentifier()));
          if (place.hasNext()) b.append(",");
        }
        b.append(";\n");

      // print input places
      b.append("  INPUT\n");
        b.append("    "); /*
        place = net.getPlaces().iterator();
        while (place.hasNext()) {
          b.append(" "+place.next());
          if (place.hasNext()) b.append(",");
        } */
        b.append(";\n");

      // print input places
      b.append("  OUTPUT\n");
        b.append("    "); /*
        place = net.getPlaces().iterator();
        while (place.hasNext()) {
          b.append(" "+place.next());
          if (place.hasNext()) b.append(",");
        } */
        b.append(";\n");
    b.append("\n");
    
    // ---------------------- markings ------------------------
    b.append("INITIALMARKING\n");
    b.append("  ");
    boolean firstPlace = true;
    for (Place p : net.getPlaces()) {
      if (p.getTokens() > 0) {
        if (!firstPlace) b.append(",");
        b.append(" "+toLoLA(p.getUniqueIdentifier())+":"+p.getTokens());
        firstPlace = false;
      }
    }
    b.append(";\n\n");
    
    b.append("FINALCONDITION\n");
    b.append("  TRUE;\n");
    b.append("\n");
    
    // ---------------------- transitions ------------------------
    for (Transition t : net.getTransitions()) {
      b.append("TRANSITION "+toLoLA(t.getUniqueIdentifier())+"\n");
        // print roles of the transition
        if (t.getRoles().size() > 0) {
          b.append("  ROLES");
          Iterator<String> role = t.getRoles().iterator();
          while (role.hasNext()) {
            b.append(" "+toLoLA(role.next()));
            if (role.hasNext()) b.append(",");
          }
          b.append(";\n");
        }
        // pre-places of the transition
        b.append("  CONSUME");
        place = t.getPreSet().iterator();
        while (place.hasNext()) {
          b.append(" "+toLoLA(place.next().getUniqueIdentifier())+":1");
          if (place.hasNext()) b.append(",");
        }
        b.append(";\n");
        // post-places of the transition
        b.append("  PRODUCE");
        place = t.getPostSet().iterator();
        while (place.hasNext()) {
          b.append(" "+toLoLA(place.next().getUniqueIdentifier())+":1");
          if (place.hasNext()) b.append(",");
        }
        b.append(";\n");
      b.append("\n");
    }
    
    return b.toString();
  }
  
  /**
   * @param format
   * @return standard file extension for given file format
   */
  public static String getFileExtension(int format) {
    switch(format) {
    case FORMAT_DOT: return "dot";
    case FORMAT_LOLA: return "lola";
    case FORMAT_OWFN: return "owfn";
    }
    return "unknown";
  }
  
  public static final int PARAM_SWIMLANE = 1;
  
  public static void writeToFile(PetriNet net, String fileName, int format, int parameter) throws IOException {

    // Create file 
    FileWriter fstream = new FileWriter(fileName+"."+getFileExtension(format));
    BufferedWriter out = new BufferedWriter(fstream);
    
    if (format == FORMAT_DOT) {
      if ((parameter & PARAM_SWIMLANE) == 1)
        out.write(net.toDot_swimlanes());
      else
        out.write(net.toDot());
    } else if (format == FORMAT_LOLA) {
      out.write(PetriNetIO.toLoLA(net));
    } else if (format == FORMAT_OWFN) {
      out.write(PetriNetIO.toOWFN(net));
    }

    //Close the output stream
    out.close();

  }
  
  private static String options_inFile = null;
  private static int options_outputFormat = 0;
  
  private static void parseCommandLine(String args[]) {
    for (int i=0; i<args.length; i++) {
      if ("-i".equals(args[i])) {
        options_inFile = args[++i];
      }
      
      if ("-f".equals(args[i])) {
        ++i;
        if ("dot".equals(args[i]))
          options_outputFormat = FORMAT_DOT;
        else if ("lola".equals(args[i]))
          options_outputFormat = FORMAT_LOLA;
        else if ("owfn".equals(args[i]))
          options_outputFormat = FORMAT_OWFN;
      }
    }
  }

  /**
   * Read Petri net from a text-file and write a GraphViz Dot representation
   * of the net to standard out.
   * 
   * @param args
   * @throws Exception
   */
  public static void main(String args[]) throws Exception {
    
    parseCommandLine(args);
    
    if (options_inFile == null) return;
    
    PetriNet net = readNetFromFile(options_inFile);
    net.anonymizeNet();
    if (net == null) System.exit(1);
    
    if (options_outputFormat == 0)
      options_outputFormat = FORMAT_DOT;
    
    writeToFile(net, options_inFile, options_outputFormat, 0);
  }
}
